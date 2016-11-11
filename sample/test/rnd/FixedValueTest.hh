/*
 * FixedValueTest.hh
 *
 * @date 23.04.2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef SAMPLE_TEST_RND_FIXEDVALUETEST_HH_
#define SAMPLE_TEST_RND_FIXEDVALUETEST_HH_

#include <gtest/gtest.h>

#include <CLHEP/Units/SystemOfUnits.h>

#include "rnd/FixedValue.hh"

class FixedValueTest: public testing::Test {
protected:

	virtual void SetUp() {
		//
	}
};

TEST_F(FixedValueTest, shootFixed) {
	rnd::FixedValue rnd(0.5, 1.0);
	for (size_t i = 0; i < 1000; i++) {
		ASSERT_TRUE(rnd.shoot() == 0.5);
	}
}

TEST_F(FixedValueTest, shootRandom) {
	rnd::FixedValue rnd(-0.5, 1.0);
	for (size_t i = 0; i < 1000; i++) {
		if(rnd.shoot() != 0.5) {
			return;
		}
	}
	FAIL();
}



#endif /* SAMPLE_TEST_RND_FIXEDVALUETEST_HH_ */
