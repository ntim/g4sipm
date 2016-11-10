/*
 * PropertiesTest.hh
 *
 * @date Mar 16, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef PROPERTIESTEST_HH_
#define PROPERTIESTEST_HH_

#include <gtest/gtest.h>
#include <fstream>
#include <iostream>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "Properties.hh"

class PropertiesTest: public testing::Test {
protected:
	Properties testedObject;
	std::string filename;

	void putKeyValuePairToTestFile(std::string key, double value) {
		std::stringstream out;
		out << key << " = " << std::setprecision(10) << value << std::endl;
		putContentToTestFile(out.str());
	}

	void putContentToTestFile(std::string content) {
		std::ofstream out(filename.c_str(), std::ios_base::out);
		out << content << std::endl;
		out.close();
	}

	virtual void SetUp() {
		filename = "test.properties";
	}
};

TEST_F(PropertiesTest, shouldFail) {
	ASSERT_FALSE(testedObject.load("non-existing-file.txt"));
}

TEST_F(PropertiesTest, shouldMatchEmptyLines) {
	putContentToTestFile("\n \n\t\n\t \n");
	ASSERT_TRUE(testedObject.load(filename));
}

TEST_F(PropertiesTest, shouldMatchComment) {
	putContentToTestFile("# Test\n\t # Test\n");
	ASSERT_TRUE(testedObject.load(filename));
}

TEST_F(PropertiesTest, shouldMatchKeyNumberValuePair) {
	putContentToTestFile("key = 0.123\nkey:0.123\n  key: 0.123\nkey=-.3\nkey=-.3e+3");
	ASSERT_TRUE(testedObject.load(filename));
}

TEST_F(PropertiesTest, shouldMatchKeyStringValuePair) {
	putContentToTestFile("key = value\n key:value\n");
	ASSERT_TRUE(testedObject.load(filename));
}

TEST_F(PropertiesTest, getNumber) {
	std::string key = "key";
	double value = 0.123;
	putKeyValuePairToTestFile(key, value);
	testedObject.load(filename);
	ASSERT_DOUBLE_EQ(value, testedObject.getNumber(key));
	//
	value = 0.123e-13;
	putKeyValuePairToTestFile(key, value);
	testedObject.load(filename);
	ASSERT_DOUBLE_EQ(value, testedObject.getNumber(key));
	//
	value = -0.123e-13;
	putKeyValuePairToTestFile(key, value);
	testedObject.load(filename);
	ASSERT_DOUBLE_EQ(value, testedObject.getNumber(key));
}

TEST_F(PropertiesTest, getString) {
	putContentToTestFile("key = value\n");
	testedObject.load(filename);
	ASSERT_STREQ("value", testedObject.getString("key").c_str());
}

TEST_F(PropertiesTest, shouldMatchTabular) {
	std::string content = "key = tabular\n"
			"	col1	col2\n"
			"	0.1	-3 \n"
			"	0.2	-2 \n"
			"	0.3	-1 \n";
	putContentToTestFile(content);
	ASSERT_TRUE(testedObject.load(filename));
}

TEST_F(PropertiesTest, getTabular) {
	std::string content = "key = tabular\n"
			"	col1	col2\n"
			"	0.1	-3 \n"
			"	0.2	-2 \n"
			"	0.3	-1.0e0 \n";
	putContentToTestFile(content);
	testedObject.load(filename);
	Properties::tabular tabular = testedObject.getTabular("key");
	ASSERT_DOUBLE_EQ(.1, boost::any_cast<double>(tabular["col1"][0]));
	ASSERT_DOUBLE_EQ(.2, boost::any_cast<double>(tabular["col1"][1]));
	ASSERT_DOUBLE_EQ(.3, boost::any_cast<double>(tabular["col1"][2]));
	ASSERT_DOUBLE_EQ(-3., boost::any_cast<double>(tabular["col2"][0]));
	ASSERT_DOUBLE_EQ(-2., boost::any_cast<double>(tabular["col2"][1]));
	ASSERT_DOUBLE_EQ(-1., boost::any_cast<double>(tabular["col2"][2]));
	ASSERT_EQ((unsigned int ) 2, tabular.size());
}

TEST_F(PropertiesTest, containsNumber) {
	putContentToTestFile("key = 0.123");
	testedObject.load(filename);
	ASSERT_TRUE(testedObject.containsNumber("key"));
	ASSERT_FALSE(testedObject.containsNumber("on-existing-key"));
}

TEST_F(PropertiesTest, containsString) {
	putContentToTestFile("key = value");
	testedObject.load(filename);
	ASSERT_TRUE(testedObject.containsString("key"));
	ASSERT_FALSE(testedObject.containsString("on-existing-key"));
}

TEST_F(PropertiesTest, containsTabular) {
	putContentToTestFile("key = tabular\n 1 2 \n");
	testedObject.load(filename);
	bool result = testedObject.containsTabular("key");
	ASSERT_TRUE(result);
	result = testedObject.containsTabular("on-existing-key");
	ASSERT_FALSE(result);
}

TEST_F(PropertiesTest, getNumberWithUnit) {
	std::string content = "num1 = 0.3 * nm \n"
			"num2 = 0.3 * %\n"
			"num3 = 1.123 * Celsius";
	putContentToTestFile(content);
	testedObject.load(filename);
	ASSERT_TRUE(testedObject.containsNumber("num1"));
	ASSERT_TRUE(testedObject.containsNumber("num2"));
	ASSERT_TRUE(testedObject.containsNumber("num3"));
	ASSERT_EQ(0.3 * CLHEP::nm, testedObject.getNumber("num1"));
	ASSERT_EQ(0.3 * CLHEP::perCent, testedObject.getNumber("num2"));
	ASSERT_EQ(1.123 * CLHEP::kelvin + CLHEP::STP_Temperature, testedObject.getNumber("num3"));
}

TEST_F(PropertiesTest, containsTabularWithUnits) {
	std::string content = "tab = tabular\n"
			"wavelength / nm	pde / %\n"
			"300	25\n";
	putContentToTestFile(content);
	testedObject.load(filename);
	ASSERT_TRUE(testedObject.containsTabular("tab"));
}

TEST_F(PropertiesTest, containsTabularWithUnitsWithoutUnits) {
	std::string content = "tab = tabular\n"
			"col1	col2\n"
			"300	25\n";
	putContentToTestFile(content);
	testedObject.load(filename);
	bool result = testedObject.containsTabular("tab");
	ASSERT_TRUE(result);
}

TEST_F(PropertiesTest, getTabularWithUnits) {
	std::string content = "tab = tabular\n"
			"wavelength / nm	pde / %\n"
			"300	25 \n"
			"400	50\n"
			"500	40\n"
			"600	30\n";
	putContentToTestFile(content);
	testedObject.load(filename);
	Properties::tabular tab = testedObject.getTabular("tab");
	ASSERT_EQ(25. * CLHEP::perCent, boost::any_cast<double>(tab["pde"][0]));
	ASSERT_EQ(50. * CLHEP::perCent, boost::any_cast<double>(tab["pde"][1]));
	ASSERT_EQ(40. * CLHEP::perCent, boost::any_cast<double>(tab["pde"][2]));
	ASSERT_EQ(30. * CLHEP::perCent, boost::any_cast<double>(tab["pde"][3]));
}

TEST_F(PropertiesTest, shouldCutCommentInLine) {
	std::string content = "key = 300 * nm # Test comment \n";
	putContentToTestFile(content);
	testedObject.load(filename);
	ASSERT_TRUE(testedObject.containsNumber("key"));
}

//TEST_F(PropertiesTest, getTabularStringToNumber) {
//	std::string content = "tab = tabular\n"
//			"col1	col2\n"
//			"one	25\n"
//			"two	50 \n"
//			"three	40 \n";
//	putContentToTestFile(content);
//	testedObject.load(filename);
//	Properties::tabular tab = testedObject.getTabular("tab");
//	ASSERT_EQ("one", boost::any_cast<std::string>(tab["col1"][0]));
//	ASSERT_EQ("two", boost::any_cast<std::string>(tab["col1"][1]));
//	ASSERT_EQ("three", boost::any_cast<std::string>(tab["col1"][2]));
//	ASSERT_EQ(25., boost::any_cast<double>(tab["col2"][0]));
//	ASSERT_EQ(50., boost::any_cast<double>(tab["col2"][1]));
//	ASSERT_EQ(40., boost::any_cast<double>(tab["col2"][2]));
//}

TEST_F(PropertiesTest, toString) {
	std::string content = "num = 0.123\n"
			"tab = tabular\n"
			"wavelength / nm	pde / %\n"
			"300	25 \n"
			"400	50\n"
			"500	40\n"
			"600	30\n";
	putContentToTestFile(content);
	testedObject.load(filename);
	testedObject.print();
	// Get all properties as string, try to persist and load again.
	putContentToTestFile(testedObject.toString());
	ASSERT_EQ(0.123, boost::any_cast<double>(testedObject.getNumber("num")));
	Properties::tabular tab = testedObject.getTabular("tab");
	ASSERT_EQ(400 * CLHEP::nanometer, boost::any_cast<double>(tab["wavelength"][1]));
	ASSERT_EQ(50 * CLHEP::perCent, boost::any_cast<double>(tab["pde"][1]));
}

#endif /* PROPERTIESTEST_HH_ */
