/*
 * VectorUtilTest.hh
 *
 * @date Feb 14, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef VECTORUTILTEST_HH_
#define VECTORUTILTEST_HH_

#include <gtest/gtest.h>

#include "VectorUtil.hh"

class VectorUtilTest: public testing::Test {
protected:

	std::vector<double> testedObject;

	virtual void SetUp() {
		for (unsigned int i = 0; i < 1000; i++) {
			testedObject.push_back(i);
		}
	}
};

TEST_F(VectorUtilTest, times) {
	std::vector<double> result = VectorUtil::times(testedObject, 2.);
	for (unsigned int i = 0; i < result.size(); i++) {
		ASSERT_DOUBLE_EQ(testedObject[i] * 2, result[i]);
	}
}

TEST_F(VectorUtilTest, adapt) {
	double array[] = { 0, 1, 2, 3, 4, 5 };
	std::vector<double> adaption = VectorUtil::adapt(array, 6);
	ASSERT_EQ((unsigned int ) 6, adaption.size());
	for (unsigned int i = 0; i < 6; i++) {
		ASSERT_DOUBLE_EQ(i, adaption[i]);
	}
}

TEST_F(VectorUtilTest, min) {
	ASSERT_EQ(0, VectorUtil::min(testedObject));
}

TEST_F(VectorUtilTest, max) {
	ASSERT_EQ(testedObject.size() - 1, VectorUtil::max(testedObject));
}

TEST_F(VectorUtilTest, mean) {
	ASSERT_EQ((testedObject.size() - 1.) / 2., VectorUtil::mean(testedObject));
}

TEST_F(VectorUtilTest, expand) {
	std::map<double, double> tabular;
	tabular[1] = .123;
	tabular[2] = .234;
	tabular[3] = .345;
	std::vector<double> x, y;
	VectorUtil::expand(tabular, &x, &y);
	ASSERT_DOUBLE_EQ(1, x[0]);
	ASSERT_DOUBLE_EQ(2, x[1]);
	ASSERT_DOUBLE_EQ(3, x[2]);
	ASSERT_DOUBLE_EQ(.123, y[0]);
	ASSERT_DOUBLE_EQ(.234, y[1]);
	ASSERT_DOUBLE_EQ(.345, y[2]);
	ASSERT_EQ((unsigned int ) 3, x.size());
	ASSERT_EQ((unsigned int ) 3, y.size());
}

TEST_F(VectorUtilTest, add) {
	double array[] = { 0, 1, 2, 3, 4, 5 };
	std::vector<double> adaption = VectorUtil::add(VectorUtil::adapt(array, 6), 3.);
	for (unsigned int i = 0; i < 6; i++) {
		ASSERT_DOUBLE_EQ(array[i] + 3., adaption[i]);
	}
}

#endif /* VECTORUTILTEST_HH_ */
