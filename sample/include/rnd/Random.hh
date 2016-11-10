/*
 * Random.hh
 *
 *  Created on: 22.04.2015
 *      Author: tim
 */

#ifndef RND_RANDOM_HH_
#define RND_RANDOM_HH_

#include <stddef.h>

namespace rnd {

template<typename T> class Random {
public:
	Random() {
		//
	}
	virtual ~Random() {

	}

	virtual T shoot() = 0;
	void shootArray(const size_t size, T* a) {
		for (T* v = a; v != a + size; ++v) {
			*v = shoot();
		}
	}
};

typedef Random<double> RandomDouble;

} /* namespace rnd */

#endif /* RND_RANDOM_HH_ */
