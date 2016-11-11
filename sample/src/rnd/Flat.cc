/*
 * Flat.cc
 *
 * @date 22.04.2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "rnd/Flat.hh"

#include <CLHEP/Random/RandFlat.h>

namespace rnd {

Flat::Flat() :
		a(0.0), b(1.0) {
	//
}

Flat::Flat(double _w) :
		a(0.0), b(_w) {
	//
}

Flat::Flat(double _a, double _b) :
		a(_a), b(_b) {
	//
}

Flat::~Flat() {
	//
}

double Flat::shoot() {
	return CLHEP::RandFlat::shoot(a, b);
}

} /* namespace random */
