/*
 * Direction.hh
 *
 * @date 23.04.2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef RND_DIRECTION_HH_
#define RND_DIRECTION_HH_

#include "rnd/ThreeVector.hh"

namespace rnd {

class Direction: public ThreeVector {
private:
	double phiMin;
	double phiMax;
	double thetaMin;
	double thetaMax;

public:
	Direction(double _phiMin, double _phiMax, double _thetaMin, double _thetaMax);
	virtual ~Direction();

	virtual CLHEP::Hep3Vector shoot();
};

} /* namespace rnd */

#endif /* RND_DIRECTION_HH_ */
