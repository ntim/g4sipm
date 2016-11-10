/*
 * DirectionTest.hh
 *
 *  Created on: 23.04.2015
 *      Author: tim
 */

#ifndef SAMPLE_TEST_RND_DIRECTIONTEST_HH_
#define SAMPLE_TEST_RND_DIRECTIONTEST_HH_

#include <gtest/gtest.h>

#include <CLHEP/Units/SystemOfUnits.h>

#include "rnd/Direction.hh"

class DirectionTest: public testing::Test {
protected:

	virtual void SetUp() {
		//
	}
};

TEST_F(DirectionTest, shoot) {
	rnd::Direction rnd(-90 * CLHEP::deg, 90 * CLHEP::deg, 0.0, 12 * CLHEP::deg);
	for (size_t i = 0; i < 1000; i++) {
		ASSERT_NEAR(rnd.shoot().mag(), 1.0, 1e-3);
	}
}

#endif /* SAMPLE_TEST_RND_DIRECTIONTEST_HH_ */
