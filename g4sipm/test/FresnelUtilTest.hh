/*
 * FresnelUtilTest.hh
 *
 * @date 26.06.2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FRESNELUTILTEST_HH_
#define FRESNELUTILTEST_HH_

#include <gtest/gtest.h>

#include <CLHEP/Units/SystemOfUnits.h>

#include "FresnelUtil.hh"

class FresnelUtilTest: public testing::Test {
protected:

	double nAir;
	double nBk7;

	virtual void SetUp() {
		nAir = 1.0002772;
		nBk7 = 1.5168;
	}
};

TEST_F(FresnelUtilTest, eta) {
	ASSERT_NEAR(0, FresnelUtil::eta(nAir, nBk7, 0.), 1e-6);
	ASSERT_NEAR(19.2525539 * CLHEP::deg, FresnelUtil::eta(nAir, nBk7, 30. * CLHEP::deg), 1e-4);
}

TEST_F(FresnelUtilTest, r) {
	ASSERT_NEAR(0.04211006596, FresnelUtil::r(nAir, nBk7, 0.), 1e-4);
}

TEST_F(FresnelUtilTest, t) {
	ASSERT_NEAR(1. - 0.04211006596, FresnelUtil::t(nAir, nBk7, 0.), 1e-4);
}

#endif /* FRESNELUTILTEST_HH_ */
