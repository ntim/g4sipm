/*
 * ThreeVector.hh
 *
 * @date 22.04.2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
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
