/*
 * FixedValue.hh
 *
 *  Created on: 23.04.2015
 *      Author: tim
 */

#ifndef RND_FIXEDVALUE_HH_
#define RND_FIXEDVALUE_HH_

#include "rnd/Flat.hh"

namespace rnd {

/**
 * Random number generator that always returns the start value if it is >= zero.
 * Otherwise, a random number is generated between zero and width.
 */
class FixedValue: public Flat {
private:
	double value;

public:
	FixedValue(double _value, double _w);
	virtual ~FixedValue();

	virtual double shoot();
};

} /* namespace rnd */

#endif /* RND_FIXEDVALUE_HH_ */
