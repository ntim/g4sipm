/*
 * Flat.hh
 *
 *  Created on: 22.04.2015
 *      Author: tim
 */

#ifndef RND_FLAT_HH_
#define RND_FLAT_HH_

#include "rnd/Random.hh"

namespace rnd {

class Flat: public Random<double> {
private:
	double a, b;

public:
	Flat();
	Flat(double _w);
	Flat(double _a, double _b);
	virtual ~Flat();

	virtual double shoot();
};

} /* namespace rnd */

#endif /* RND_FLAT_HH_ */
