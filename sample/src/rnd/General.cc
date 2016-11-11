/*
 * General.cc
 *
 * @date 22.04.2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "rnd/General.hh"

#include <stdexcept>
#include <algorithm>
#include <cmath>

#include <CLHEP/Random/RandFlat.h>

#include "Properties.hh"

namespace rnd {

General::General(std::vector<double> _x, std::vector<double> _pdf) :
		x(_x), pdf(_pdf) {
	normalizePdf();
}

General::General(std::string filename) {
	Properties p;
	p.load(filename);
	if (p.containsTabular("pdf")) {
		Properties::tabular tab = p.getTabular("pdf");
		if (tab.count("x") && tab.count("pdf")) {
			x = tab["x"];
			pdf = tab["pdf"];
		}
	}
	normalizePdf();
}

General::~General() {
	//
}

void General::normalizePdf() {
	// TODO sort values.
	if (x.size() != pdf.size()) {
		throw std::runtime_error("x and pdf dimension are not equal");
	}
	if (pdf.size() == 0) {
		throw std::runtime_error("empty pdf");
	}
	double max = *std::max_element(pdf.begin(), pdf.end());
	for (std::vector<double>::iterator it = pdf.begin(); it != pdf.end(); it++) {
		*it /= max;
		if (std::isnan(*it)) {
			throw std::runtime_error("encounterd nan");
		}
		if ((*it) < 0.0) {
			throw std::runtime_error("encountered negative value");
		}
	}
	// Check if there is at least one element not zero.
	for (std::vector<double>::iterator it = pdf.begin(); it != pdf.end(); it++) {
		if ((*it) != 0.0) {
			return;
		}
	}
	throw std::runtime_error("all elements are zero");
}

double General::shoot() {
	size_t n = 0;
	while ((n++) < 10000 * pdf.size()) {
		// Dice index of the nonzero PDF array.
		size_t i = CLHEP::RandFlat::shootInt((long int) pdf.size());
		if (pdf.at(i) > 0.0) {
			double pi = CLHEP::RandFlat::shoot();
			if (pi <= pdf.at(i)) {
				// Interpolate between bins size.
				if (i < pdf.size() - 1) {
					return CLHEP::RandFlat::shoot(x.at(i), x.at(i + 1));
				}
				return x.at(i);
			}
		}
	}
	throw std::runtime_error("random::General::shoot(): could not find a random number");
}

void General::print() const {
	for (size_t i = 0; i < x.size(); i++) {
		std::cout << "RandGeneral::print(): (x, pdf) = (" << x[i] << " ns, " << pdf[i] << ")." << std::endl;
	}
}

double General::getXMin() const {
	return *std::min_element(x.begin(), x.end());
}

double General::getXMax() const {
	return *std::max_element(x.begin(), x.end());
}

} /* namespace rnd */

