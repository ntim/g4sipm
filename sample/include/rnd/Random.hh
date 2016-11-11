/*
 * Random.hh
 *
 * @date 22.04.2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
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
