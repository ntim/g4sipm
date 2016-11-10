/*
 * Flat.cc
 *
 *  Created on: 22.04.2015
 *      Author: tim
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
