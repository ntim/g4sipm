/*
 * Direction.cc
 *
 * @date 23.04.2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "rnd/Direction.hh"

#include <cmath>
#include <CLHEP/Random/RandFlat.h>

#include "rnd/Flat.hh"

namespace rnd {

Direction::Direction(double _phiMin, double _phiMax, double _thetaMin, double _thetaMax) :
		ThreeVector(new Flat()), phiMin(_phiMin), phiMax(_phiMax), thetaMin(_thetaMin), thetaMax(_thetaMax) {
	//
}

Direction::~Direction() {
	//
}

CLHEP::Hep3Vector Direction::shoot() {
	double phi = CLHEP::RandFlat::shoot(phiMin, phiMax);
	double cosTheta = CLHEP::RandFlat::shoot(cos(thetaMin), cos(thetaMax));
	double sinTheta = sqrt(1. - cosTheta * cosTheta);
	return CLHEP::Hep3Vector(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
}

} /* namespace random */
