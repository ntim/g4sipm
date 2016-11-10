/*
 * InterpolationUtilTest.h
 *
 * @date Feb 14, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef INTERPOLATIONUTILTEST_H_
#define INTERPOLATIONUTILTEST_H_

#include <gtest/gtest.h>
#include <math.h>

#include "InterpolationUtil.hh"

class InterpolationUtilTest: public testing::Test {
protected:
	std::vector<double> x;
	std::vector<double> y;
	std::map<double, double> xy;

	double function(const double a) const {
		return 1.5 * a + 2.;
	}

	virtual void SetUp() {
		for (unsigned int i = 0; i < 100; i++) {
			x.push_back(i);
			y.push_back(function(i));
			xy[i] = function(i);
		}
	}
};

TEST_F(InterpolationUtilTest, linearInside) {
	ASSERT_DOUBLE_EQ(function(1), InterpolationUtil::linear(x, y, 1));
}

TEST_F(InterpolationUtilTest, linearBetween) {
	ASSERT_DOUBLE_EQ(function(1.5), InterpolationUtil::linear(x, y, 1.5));
	ASSERT_DOUBLE_EQ(function(2.25), InterpolationUtil::linear(x, y, 2.25));
}

TEST_F(InterpolationUtilTest, linearBelow) {
	ASSERT_DOUBLE_EQ(function(-100), InterpolationUtil::linear(x, y, -100));
}

TEST_F(InterpolationUtilTest, linearBeyond) {
	ASSERT_DOUBLE_EQ(function(1000), InterpolationUtil::linear(x, y, 1000));
}

TEST_F(InterpolationUtilTest, linearInsideMap) {
	ASSERT_DOUBLE_EQ(function(1), InterpolationUtil::linear(xy, 1));
}

TEST_F(InterpolationUtilTest, linearBetweenMap) {
	ASSERT_DOUBLE_EQ(function(1.5), InterpolationUtil::linear(xy, 1.5));
	ASSERT_DOUBLE_EQ(function(2.25), InterpolationUtil::linear(xy, 2.25));
}

TEST_F(InterpolationUtilTest, linearBelowMap) {
	ASSERT_DOUBLE_EQ(function(-100), InterpolationUtil::linear(xy, -100));
}

TEST_F(InterpolationUtilTest, linearBeyondMap) {
	ASSERT_DOUBLE_EQ(function(1000), InterpolationUtil::linear(xy, 1000));
}

#endif /* INTERPOLATIONUTILTEST_H_ */
