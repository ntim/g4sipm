/*
 * G4SipmConfigFileModel.cc
 *
 * @date Mar 13, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/impl/G4SipmConfigFileModel.hh"

#include <set>
#include <algorithm>
#include <iomanip>
#include <boost/format.hpp>

#include <CLHEP/Units/SystemOfUnits.h>

#include "MaterialFactory.hh"
#include "InterpolationUtil.hh"
#include "VectorUtil.hh"
#include "G4SipmUiMessenger.hh"
#include "model/impl/G4SipmRandomGainMapModel.hh"
#include "model/impl/G4SipmConfigFileVoltageTraceModel.hh"

G4SipmConfigFileModel::G4SipmConfigFileModel(std::string filename) :
		G4SipmModel(new G4SipmRandomGainMapModel, new G4SipmConfigFileVoltageTraceModel(filename)) {
	// Load the properties.
	if (!p.load(filename)) {
		throw 1;
	}
	// Initialize fields.
	initialize();
	// Set operating point.
	setBiasVoltage(p.getNumber("biasVoltage"));
	setTemperature(p.getNumber("temperature"));
	//
	getGainMapModel()->refresh(this);
}

void G4SipmConfigFileModel::initialize() {
	name = p.getString("name");
	numberOfCells = static_cast<int>(p.getNumber("numberOfCells"));
	cellPitch = p.getNumber("cellPitch");
	fillFactor = p.getNumber("fillFactor");
	thickness = p.getNumber("thickness");
	windowThickness = p.getNumber("windowThickness");
	deadTime = p.getNumber("deadTime");
	gainVariation = p.getNumber("gainVariation");
	entry = p.getTabular("operatingParameters")["entry"];
	temperature = p.getTabular("operatingParameters")["temperature"];
	overVoltage = p.getTabular("operatingParameters")["overVoltage"];
	breakDownVoltage = p.getTabular("operatingParameters")["breakDownVoltage"];
	thermalNoiseRate = p.getTabular("operatingParameters")["thermalNoiseRate"];
	crossTalkProbability = p.getTabular("operatingParameters")["crossTalkProbability"];
	afterPulseProbLong = p.getTabular("operatingParameters")["afterPulseProbLong"];
	afterPulseProbShort = p.getTabular("operatingParameters")["afterPulseProbShort"];
	afterPulseTauLong = p.getTabular("operatingParameters")["afterPulseTauLong"];
	afterPulseTauShort = p.getTabular("operatingParameters")["afterPulseTauShort"];
	recoveryTime = p.getTabular("operatingParameters")["recoveryTime"];
	// Sanity check.
	if (temperature.size() == 0) {
		std::cerr << "G4SipmConfigFileModel::initialize(): no operating points found." << std::endl;
		throw 1;
	}
	// Initialize photon detection efficiency.
	initializePde();
	// Initialize window material
	initializeWindowMaterial();
}

void G4SipmConfigFileModel::initializePde() {
	// Fetch pde tabular.
	std::vector<double> pdeEntry = p.getTabular("photonDetectionEfficiency")["entry"];
	std::vector<double> wvl = p.getTabular("photonDetectionEfficiency")["wavelength"];
	std::vector<double> eff = p.getTabular("photonDetectionEfficiency")["efficiency"];
	// Determine unique entry numbers.
	std::set<double> unique(pdeEntry.begin(), pdeEntry.end());
	// Check if operating point entry ids match operating point entry ids.
	std::set<double> uniqueOp(entry.begin(), entry.end());
	if (unique.size() != uniqueOp.size()) {
		std::cerr << "G4SipmConfigFileModel::initializePde(): operation point entry IDs do not match PDE entry IDs."
				<< std::endl;
		throw 1;
	}
	// For each unique entry...
	for (std::set<double>::iterator it = unique.begin(); it != unique.end(); it++) {
		std::vector<double> x, y;
		// ... search all occurrences of the entry id.
		for (unsigned int j = 0; j < pdeEntry.size(); j++) {
			if (pdeEntry[j] == (unsigned int) *it) {
				x.push_back(wvl[j]);
				y.push_back(eff[j]);
			}
		}
		pdeWvl.push_back(x);
		pdeEff.push_back(y);
	}
	// Sanity check.
	for (unsigned int i = 0; i < pdeWvl.size(); i++) {
		if (pdeWvl[i].size() == 0) {
			std::cerr << "G4SipmConfigFileModel::initializePde(): PDE entry " << i << " has no data." << std::endl;
			throw 1;
		}
	}
	// Reorder entry IDs.
	std::vector<double> uniqueArray(unique.begin(), unique.end());
	std::sort(uniqueArray.begin(), uniqueArray.end());
	std::vector<double> entryReordered;
	for (std::vector<double>::iterator it = entry.begin(); it != entry.end(); it++) {
		// Lookup position of the id in the array.
		std::vector<double>::iterator pos = std::find(uniqueArray.begin(), uniqueArray.end(), *it);
		if (pos == entry.end()) {
			std::cerr << "G4SipmConfigFileModel::initializePde(): failed to reorder entry indices." << std::endl;
			throw 1;
		}
		// Add new entry index.
		entryReordered.push_back(std::distance(uniqueArray.begin(), pos));
	}
	entry = entryReordered;
}

void G4SipmConfigFileModel::initializeWindowMaterial() {
	windowMaterial = new G4Material(boost::str(boost::format("%s-windowMaterial") % getName()),
			1.0 * CLHEP::g / CLHEP::cm3, MaterialFactory::getInstance()->getEpoxy());
	double energies[] = { 1 * CLHEP::eV, 10 * CLHEP::eV };
	double rindex = p.getNumber("windowRefractiveIndex");
	double indices[] = { rindex, rindex };
	// Set material properties table.
	G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
	mpt->AddProperty("RINDEX", energies, indices, 2);
	windowMaterial->SetMaterialPropertiesTable(mpt);
}

void G4SipmConfigFileModel::updateOperatingPoint() {
	// If only one entry, operating point is fixed.
	if (temperature.size() == 1) {
		op = operatingpoint(0, 0);
		return;
	}
	// Remember desired temperature.
	double desiredT = getTemperature();
	std::vector<double> distances;
	for (unsigned int i = 0; i < temperature.size(); i++) {
		double t = temperature[i];
		double vOv = overVoltage[i];
		double vOvCalc = getBiasVoltage() - breakDownVoltage[i];
		// Optimal operating point would be at desiredT, vOvCalc.
		// Calculate the distance to this point.
		double dist = sqrt((t - desiredT) * (t - desiredT) + (vOv - vOvCalc) * (vOv - vOvCalc));
		// Debug output.
		if (G4SipmUiMessenger::getInstance()->getVerbose() > 2) {
			std::cout << "G4SipmConfigFileModel::updateOperatingPoint(): Op " << i << ": T = " << t / CLHEP::kelvin
					<< " K, T_set = " << desiredT << " K, V_b = " << breakDownVoltage[i] / CLHEP::volt << " V, V_ov = "
					<< vOv / CLHEP::volt << " V, V_ov,calc = " << vOvCalc / CLHEP::volt << " V, dist = "
					<< std::setprecision(12) << dist << "." << std::endl;
		}
		// If the distance is zero, an exact match has been found.
		if (dist == 0.0) {
			op = operatingpoint(i, i);
			if (G4SipmUiMessenger::getInstance()->getVerbose() > 1) {
				std::cout << "G4SipmConfigFileModel::updateOperatingPoint(): chose (" << i << ") for T = "
						<< desiredT / CLHEP::kelvin - CLHEP::STP_Temperature << " Celsius and V_bias = "
						<< getBiasVoltage() / CLHEP::volt << " V;" << std::endl;
			}
			return;
		}
		//
		distances.push_back(dist);
	}
	// Find smallest entry.
	std::vector<double>::iterator minElement = std::min_element(distances.begin(), distances.end());
	unsigned int op1 = std::distance(distances.begin(), minElement);
	// Set smallest element to a highest value possible.
	distances[op1] = DBL_MAX;
	// Find second smallest entry.
	minElement = std::min_element(distances.begin(), distances.end());
	unsigned int op2 = std::distance(distances.begin(), minElement);
	if (G4SipmUiMessenger::getInstance()->getVerbose() > 1) {
		std::cout << "G4SipmConfigFileModel::updateOperatingPoint(): chose (" << op1 << ", " << op2 << ") for T = "
				<< desiredT / CLHEP::kelvin - CLHEP::STP_Temperature << " Celsius and V_bias = "
				<< getBiasVoltage() / CLHEP::volt << " V;" << std::endl;
	}
	op = operatingpoint(op1, op2);
}

double G4SipmConfigFileModel::interpolate(const std::vector<double>& quantity) const {
	// Get operating point.
	unsigned int op1, op2;
	boost::tie(op1, op2) = op;
	if (op1 == op2) {
		return quantity[op1];
	}
	// Map quantity in dependence to the temperature.
	double t[] = { temperature[op1], temperature[op2] };
	double vb[] = { breakDownVoltage[op1] + overVoltage[op1], breakDownVoltage[op2] + overVoltage[op2] };
	double q[] = { quantity[op1], quantity[op2] };
	// If temperature varies...
	if (t[0] != t[1]) {
		return InterpolationUtil::linear(std::vector<double>(t, t + 2), std::vector<double>(q, q + 2), getTemperature());
	}
	if (vb[0] != vb[1]) {
		return InterpolationUtil::linear(std::vector<double>(vb, vb + 2), std::vector<double>(q, q + 2),
				getBiasVoltage());
	}
	return (q[0] + q[1]) / 2.;
}

double G4SipmConfigFileModel::getBreakdownVoltage() const {
	return interpolate(breakDownVoltage);
}

double G4SipmConfigFileModel::getPhotonDetectionEfficiency(double wavelength) const {
	unsigned int op1, op2;
	boost::tie(op1, op2) = op;
	// Find which PDE entries have to be taken into account.
	unsigned int entry1 = entry[op1];
	unsigned int entry2 = entry[op2];
	// No interpolation between operating points needed.
	if (entry1 == entry2) {
		return InterpolationUtil::linear(pdeWvl[entry1], pdeEff[entry1], wavelength);
	}
	// Interpolate between operation points.
	double pde[] = { InterpolationUtil::linear(pdeWvl[entry1], pdeEff[entry1], wavelength), InterpolationUtil::linear(
			pdeWvl[entry2], pdeEff[entry2], wavelength) };
	double t[] = { temperature[op1], temperature[op2] };
	double vb[] = { breakDownVoltage[op1] + overVoltage[op1], breakDownVoltage[op2] + overVoltage[op2] };
	if (t[0] != t[1]) {
		return InterpolationUtil::linear(std::vector<double>(t, t + 2), std::vector<double>(pde, pde + 2),
				getTemperature());
	}
	if (vb[0] != vb[1]) {
		return InterpolationUtil::linear(std::vector<double>(vb, vb + 2), std::vector<double>(pde, pde + 2),
				getBiasVoltage());
	}
	return (pde[0] + pde[1]) / 2.;
}

std::string G4SipmConfigFileModel::getName() const {
	return name;
}

unsigned int G4SipmConfigFileModel::getNumberOfCells() const {
	return numberOfCells;
}

double G4SipmConfigFileModel::getCellPitch() const {
	return cellPitch;
}

double G4SipmConfigFileModel::getFillFactor() const {
	return fillFactor;
}

double G4SipmConfigFileModel::getThermalNoiseRate() const {
	return interpolate(thermalNoiseRate);
}

double G4SipmConfigFileModel::getCrossTalkProbability() const {
	return interpolate(crossTalkProbability);
}

double G4SipmConfigFileModel::getApProbLong() const {
	return interpolate(afterPulseProbLong);
}

double G4SipmConfigFileModel::getApProbShort() const {
	return interpolate(afterPulseProbShort);
}

double G4SipmConfigFileModel::getApTauLong() const {
	return interpolate(afterPulseTauLong);
}

double G4SipmConfigFileModel::getApTauShort() const {
	return interpolate(afterPulseTauShort);
}

double G4SipmConfigFileModel::getRecoveryTime() const {
	return interpolate(recoveryTime);
}

void G4SipmConfigFileModel::setBiasVoltage(double _biasVoltage) {
	G4SipmModel::setBiasVoltage(_biasVoltage);
	updateOperatingPoint();
}

void G4SipmConfigFileModel::setTemperature(double _temperature) {
	G4SipmModel::setTemperature(_temperature);
	updateOperatingPoint();
}

double G4SipmConfigFileModel::getThickness() const {
	return thickness;
}

G4Material* G4SipmConfigFileModel::getWindowMaterial() const {
	return windowMaterial;
}

double G4SipmConfigFileModel::getWindowThickness() const {
	return windowThickness;
}

double G4SipmConfigFileModel::getDeadTime() const {
	return deadTime;
}

double G4SipmConfigFileModel::getGainVariation() const {
	return gainVariation;
}
