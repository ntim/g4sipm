/*
 * ThreeVectorTest.hh
 *
 *  Created on: 23.04.2015
 *      Author: tim
 */

#ifndef SAMPLE_TEST_RND_THREEVECTORTEST_HH_
#define SAMPLE_TEST_RND_THREEVECTORTEST_HH_

#include <gtest/gtest.h>

#include <CLHEP/Units/SystemOfUnits.h>

#include "rnd/ThreeVector.hh"
#include "rnd/Flat.hh"

class ThreeVectorTest: public testing::Test {
protected:

	virtual void SetUp() {
		//
	}
};

TEST_F(ThreeVectorTest, shoot) {
	rnd::ThreeVector rnd(new rnd::Flat(1.0));
	for (size_t i = 0; i < 1000; i++) {
		CLHEP::Hep3Vector v = rnd.shoot();
		ASSERT_TRUE(v.x() < 1.0);
		ASSERT_TRUE(v.y() < 1.0);
		ASSERT_TRUE(v.z() < 1.0);
	}
}

TEST_F(ThreeVectorTest, shootDifferentComponents) {
	rnd::ThreeVector rnd(new rnd::Flat(1.0), new rnd::Flat(-1.0, 0.0), new rnd::Flat(0.5));
	for (size_t i = 0; i < 1000; i++) {
		CLHEP::Hep3Vector v = rnd.shoot();
		ASSERT_TRUE(v.x() < 1.0);
		ASSERT_TRUE(v.y() < 0.0 && v.y() >= -1.0);
		ASSERT_TRUE(v.z() < 0.5);
	}
}

#endif /* SAMPLE_TEST_RND_THREEVECTORTEST_HH_ */
