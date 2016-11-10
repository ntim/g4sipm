/*
 * FixedValue.cc
 *
 *  Created on: 23.04.2015
 *      Author: tim
 */

#include <rnd/FixedValue.hh>

namespace rnd {

FixedValue::FixedValue(double _value, double _w) :
		Flat(_w), value(_value) {
	//
}

FixedValue::~FixedValue() {
	//
}

double FixedValue::shoot() {
	if (value < 0.0) {
		return Flat::shoot();
	}
	return value;
}

} /* namespace random */
