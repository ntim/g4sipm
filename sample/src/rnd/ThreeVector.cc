/*
 * ThreeVector.cc
 *
 * @date 22.04.2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <rnd/ThreeVector.hh>

namespace rnd {

ThreeVector::ThreeVector(RandomDouble* r) :
		x(r), y(r), z(r) {
	//
}

ThreeVector::ThreeVector(RandomDouble* _x, RandomDouble* _y, RandomDouble* _z) :
		x(_x), y(_y), z(_z) {
	//
}

ThreeVector::~ThreeVector() {
	if (x == y && y == z) {
		delete x;
	} else if (x == y && y != z) {
		delete x;
		delete z;
	} else if (x == z && y != z) {
		delete y;
		delete z;
	} else {
		delete x;
		delete y;
		delete z;
	}
}

CLHEP::Hep3Vector ThreeVector::shoot() {
	return CLHEP::Hep3Vector(x->shoot(), y->shoot(), z->shoot());
}

} /* namespace random */
