/*
 * Properties.cc
 *
 * @date Mar 16, 2012
 * @author Erik Dietz-Laursonn, Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "Properties.hh"

#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>

#include <G4UnitsTable.hh>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include <math.h>
#include <cmath>
#include <iostream>
#include <fstream>

Properties::Properties() {
	std::string spacePattern = "[[:space:]]*";
	std::string stringPattern = "([a-zA-Z0-9_-]+)";
	std::string numberPattern = "([+-]?[0-9]*[.]?[0-9]*[eE]?[+-]?[0-9]*)";
	std::string stringOrNumberPattern = "([a-zA-Z0-9_-]+|[+-]?[0-9]*[.]?[0-9]*[eE]?[+-]?[0-9]*)";
	std::string keyPattern = stringPattern + spacePattern + "[=:]{1}" + spacePattern;
	std::string unitPattern = spacePattern + "([1a-zA-Z%]{1}[a-zA-Z0-9/]*)" + spacePattern;
	// Define matchers.
	emptyRx = boost::regex("[^[:w:]]*");
	commentRx = boost::regex("[^[:w:]]*#+.*");
	commentInLineRx = boost::regex("([^#]+)(#.*)");
	numberRx = boost::regex(spacePattern + numberPattern + spacePattern);
	keyNumberValuePairRx = boost::regex(keyPattern + numberPattern + spacePattern);
	keyNumberValueUnitTripletRx = boost::regex(keyPattern + numberPattern + spacePattern + "[*]{1}" + unitPattern);
	keyStringValuePairRx = boost::regex(keyPattern + stringPattern + spacePattern);
	tabularStartRx = boost::regex(keyPattern + "(tabular)" + spacePattern);
	descriptorRx = boost::regex(stringPattern + spacePattern + "/" + unitPattern);
}

Properties::~Properties() {
	//
}

bool Properties::matchesCelsius(std::string str) {
	return str == "Celsius";
}

bool Properties::matchesPerCent(std::string str) {
	return (str == "perCent" || str == "%");
}

double Properties::convert(double value, std::string unitStr) {
	// Match empty unit string.
	if (unitStr == "") {
		return value;
	}
	// Try to match Celsius.
	if (matchesCelsius(unitStr)) {
		return value * G4UnitDefinition::GetValueOf("K") + CLHEP::STP_Temperature;
	}
	// Try to match %.
	if (matchesPerCent(unitStr)) {
		return value * CLHEP::perCent;
	}
	// Match with Geant4.
	return value * G4UnitDefinition::GetValueOf(G4String(unitStr));
}

void Properties::parseTabular(std::ifstream* in, std::string key) {
	tabular tab;
	// Temporary container.
	std::vector<std::string> columnNames;
	std::vector<std::string> columnUnits;
	std::vector<std::vector<double> > columns;
	std::string line;
	boost::match_results<std::string::const_iterator> result;
	// Match header line.
	getline(*in, line);
	// Match comment at line end and cut it off.
	if (boost::regex_search(line, result, commentInLineRx, boost::match_all)) {
		line = result[1];
	}
	// Tokenizer definition.
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep("\t");
	// Parse the header line.
	tokenizer headerTokens(line, sep);
	for (tokenizer::iterator it = headerTokens.begin(); it != headerTokens.end(); ++it) {
		std::string descriptor = (std::string)(*it);
		// Matched descriptor with unit.
		boost::match_results<std::string::const_iterator> regexResult;
		if (boost::regex_match(descriptor, regexResult, descriptorRx, boost::match_all)) {
			columnNames.push_back(regexResult[1]);
			columnUnits.push_back(regexResult[2]);
		} else {
			// Matched descriptor without.
			columnNames.push_back(descriptor);
			columnUnits.push_back("");
		}
		columns.push_back(std::vector<double>());
	}
	// Exit if the header line could not be resolved.
	if (columnNames.size() == 0) {
		std::cerr << "Properties::parseTabular(in, key=\"" << key << "\"): could not parse tabular " << std::endl;
		return;
	}
	// Parse table content.
	while (getline(*in, line)) {
		// Match comment at line end and cut it off.
		if (boost::regex_search(line, result, commentInLineRx, boost::match_all)) {
			line = result[1];
		}

		tokenizer lineTokens(line, sep);
		// Determine number of tokens.
		unsigned int size = 0;
		for (tokenizer::iterator it = lineTokens.begin(); it != lineTokens.end(); ++it) {
			size++;
		}
		// Skip if the line was empty or the number of tokens mismatches the number of columns.
		if (line.size() == 0 || size != columns.size()) {
			// TODO(tim): reset file position.
			break;
		}
		// Insert values.
		int colId = 0;
		for (tokenizer::iterator it = lineTokens.begin(); it != lineTokens.end(); ++it) {
			std::string cell = (std::string)(*it);
			// Match number entry.
			if (regex_match(cell, result, numberRx)) {
				double value = atof(cell.c_str());
				columns[colId].push_back(convert(value, columnUnits[colId]));
			} else {
				std::cerr << "Properties::parseTabular(in, key=\"" << key << "\"): could not parse cell " << cell
						<< " in line " << colId << std::endl;
			}
			colId++;
		}
	}
	// Build up table.
	for (unsigned int i = 0; i < columnNames.size(); ++i) {
		tab[columnNames[i]] = columns[i];
	}
	entries[key] = tab;
}

bool Properties::load(std::string _filename) {
	entries.clear();
	// Open the file.
	std::ifstream in(_filename.c_str());
	if (!in.is_open()) {
		std::cerr << "Properties::load(filename = \"" << _filename << "\"): can not open file." << std::endl;
		return false;
	}
	// Iterate over all lines in the file.
	std::string line;
	boost::match_results<std::string::const_iterator> result;
	while (getline(in, line)) {
		// Match empty line.
		if (line.empty() || boost::regex_match(line, emptyRx)) {
			continue;
		}
		// Match comment line.
		if (regex_match(line, commentRx)) {
			continue;
		}
		// Match comment at line end and cut it off.
		if (boost::regex_search(line, result, commentInLineRx, boost::match_all)) {
			line = result[1];
		}
		// Match key-number-pair.
		if (boost::regex_search(line, result, keyNumberValuePairRx, boost::match_all)) {
			std::string key = result[1];
			std::string value = result[2];
			entries[key] = static_cast<double>(atof(value.c_str()));
			continue;
		}
		// Match key-number-unit-triplet.
		if (regex_search(line, result, keyNumberValueUnitTripletRx, boost::match_all)) {
			std::string key = result[1];
			std::string value = result[2];
			std::string unit = result[3];
			entries[key] = convert(static_cast<double>(atof(value.c_str())), unit);
			continue;
		}
		// Match tabular data.
		if (boost::regex_search(line, result, tabularStartRx, boost::match_all)) {
			parseTabular(&in, result[1]);
			continue;
		}
		// Match key-string-pair.
		if (boost::regex_search(line, result, keyStringValuePairRx, boost::match_all)) {
			std::string key = result[1];
			std::string value = result[2];
			entries[key] = value;
			continue;
		}
		// Line could not be matched, so break operation with an error.
		std::cerr << "Properties::load(filename = \"" << _filename << "\"): can not match \"" << line << "\"."
				<< std::endl;
		return false;
	}
	filename = _filename;
	return true;
}

double Properties::getNumber(std::string key) const {
	if (!containsNumber(key)) {
		std::cerr << "Properties::getNumber(key = \"" << key << "\"): does not exist." << std::endl;
		return NAN;
	}
	return boost::any_cast<double>(entries.at(key));
}

std::string Properties::getString(std::string key) const {
	if (!containsString(key)) {
		std::cerr << "Properties::getString(key = \"" << key << "\"): does not exist." << std::endl;
		return "";
	}
	return boost::any_cast<std::string>(entries.at(key));
}

bool Properties::containsNumber(std::string key) const {
	try {
		boost::any_cast<double>(entries.at(key));
		return true;
	} catch (...) {
		//
	}
	return false;
}

bool Properties::containsString(std::string key) const {
	try {
		boost::any_cast<std::string>(entries.at(key));
		return true;
	} catch (...) {
		//
	}
	return false;
}

Properties::tabular Properties::getTabular(std::string key) const {
	if (!containsTabular(key)) {
		std::cerr << "Properties::getTabular(key = \"" << key << "\"): does not exist." << std::endl;
		return tabular();
	}
	return boost::any_cast<tabular>(entries.at(key));
}

std::string Properties::toString() const {
	std::stringstream out;
	// Iterate over all properties.
	for (std::map<std::string, boost::any>::const_iterator it = entries.begin(); it != entries.end(); it++) {
		out << it->first << ":\t";
		const std::type_info& type = it->second.type();
		if (type == typeid(double)) {
			out << boost::any_cast<double>(it->second);
		} else if (type == typeid(std::string)) {
			out << boost::any_cast<std::string>(it->second);
		} else if (type == typeid(tabular)) {
			out << "tabular\n";
			tabular tab = boost::any_cast<tabular>(it->second);
			// Determine number of entries.
			unsigned int n = tab.begin()->second.size();
			// Print column names.
			out << "\t";
			for (tabular::const_iterator colIt = tab.begin(); colIt != tab.end(); colIt++) {
				out << colIt->first << "\t";
			}
			out << "\n";
			// Print values.
			for (unsigned int i = 0; i < n; i++) {
				out << "\t";
				for (tabular::const_iterator colIt = tab.begin(); colIt != tab.end(); colIt++) {
					boost::any entry = colIt->second.at(i);
					if (entry.type() == typeid(double)) {
						out << boost::any_cast<double>(entry);
					} else if (entry.type() == typeid(std::string)) {
						out << boost::any_cast<std::string>(entry);
					}
					out << "\t";
				}
				out << "\n";
			}
		}
		out << "\n";
	}
	return out.str();
}

void Properties::print() const {
	std::cout << toString() << std::endl;
}

bool Properties::containsTabular(std::string key) const {
	try {
		boost::any_cast<tabular>(entries.at(key));
		return true;
	} catch (...) {
		//
	}
	return false;
}

