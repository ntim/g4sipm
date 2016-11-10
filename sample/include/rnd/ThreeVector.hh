/*
 * ThreeVector.hh
 *
 *  Created on: 22.04.2015
 *      Author: tim
 */

#ifndef RND_THREEVECTOR_HH_
#define RND_THREEVECTOR_HH_

#include "rnd/Random.hh"

#include <CLHEP/Vector/ThreeVector.h>

namespace rnd {

class ThreeVector: public Random<CLHEP::Hep3Vector> {
private:
	RandomDouble* x;
	RandomDouble* y;
	RandomDouble* z;

public:
	ThreeVector(RandomDouble* r);
	ThreeVector(RandomDouble* _x, RandomDouble* _y, RandomDouble* _z);
	virtual ~ThreeVector();

	virtual CLHEP::Hep3Vector shoot();
};

} /* namespace rnd */

#endif /* RND_THREEVECTOR_HH_ */
