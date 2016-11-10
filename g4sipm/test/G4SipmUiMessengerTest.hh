/*
 * G4SipmUiMessengerTest.hh
 *
 * @date 23.08.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMUIMESSENGERTEST_HH_
#define G4SIPMUIMESSENGERTEST_HH_

#include <gtest/gtest.h>
#include <string>
#include <fstream>

#include "G4SipmUiMessenger.hh"
#include "G4UiMessengerUtil.hh"

class G4SipmUiMessengerTest: public testing::Test {
protected:
	virtual void SetUp() {
		G4SipmUiMessenger::getInstance()->setDefaults();
		// Put a macro file which the messenger can find.
		// Values should not be default
		std::string macro = "/g4sipm/verbose 3\n"
				"/g4sipm/filter/geometry 0\n"
				"/g4sipm/filter/pde 0\n"
				"/g4sipm/filter/timing 0\n"
				"/g4sipm/noise/thermal 0\n"
				"/g4sipm/noise/preThermal 12\n"
				"/g4sipm/noise/postThermal 12\n"
				"/g4sipm/noise/afterpulse 0\n"
				"/g4sipm/noise/crosstalk 0\n"
				"/g4sipm/noise/ifNoSignal 1\n"
				"/g4sipm/digitize/hits 0\n"
				"/g4sipm/digitize/trace 0\n";
		std::ofstream out(G4SipmUiMessenger::MACRO_FILE_NAME.c_str());
		out << macro;
		out.close();
		// Execute macro.
		G4UiMessengerUtil::executeMacro(G4SipmUiMessenger::MACRO_FILE_NAME);
	}

	virtual void TearDown() {
		G4SipmUiMessenger::getInstance()->setDefaults();
	}

};

TEST_F(G4SipmUiMessengerTest, getVerbose) {
	ASSERT_EQ(3, G4SipmUiMessenger::getInstance()->getVerbose());
}

TEST_F(G4SipmUiMessengerTest, isFilterGeometry) {
	ASSERT_FALSE(G4SipmUiMessenger::getInstance()->isFilterGeometry());
}

TEST_F(G4SipmUiMessengerTest, isFilterPde) {
	ASSERT_FALSE(G4SipmUiMessenger::getInstance()->isFilterPde());
}

TEST_F(G4SipmUiMessengerTest, isFilterTiming) {
	ASSERT_FALSE(G4SipmUiMessenger::getInstance()->isFilterTiming());
}

TEST_F(G4SipmUiMessengerTest, isNoiseThermal) {
	ASSERT_FALSE(G4SipmUiMessenger::getInstance()->isNoiseThermal());
}

TEST_F(G4SipmUiMessengerTest, getNoisePreThermal) {
	ASSERT_EQ((unsigned int) 12, G4SipmUiMessenger::getInstance()->getNoisePreThermal());
}

TEST_F(G4SipmUiMessengerTest, getNoisePostThermal) {
	ASSERT_EQ((unsigned int) 12, G4SipmUiMessenger::getInstance()->getNoisePostThermal());
}

TEST_F(G4SipmUiMessengerTest, isNoiseAfterpulse) {
	ASSERT_FALSE(G4SipmUiMessenger::getInstance()->isNoiseAfterpulse());
}

TEST_F(G4SipmUiMessengerTest, isNoiseCrosstalk) {
	ASSERT_FALSE(G4SipmUiMessenger::getInstance()->isNoiseCrosstalk());
}

TEST_F(G4SipmUiMessengerTest, isNoiseIfNoSignal) {
	ASSERT_TRUE(G4SipmUiMessenger::getInstance()->isNoiseIfNoSignal());
}

TEST_F(G4SipmUiMessengerTest, isDigitizeHits) {
	ASSERT_FALSE(G4SipmUiMessenger::getInstance()->isDigitizeHits());
}

TEST_F(G4SipmUiMessengerTest, isDigitizeTrace) {
	ASSERT_FALSE(G4SipmUiMessenger::getInstance()->isDigitizeTrace());
}

#endif /* G4SIPMUIMESSENGERTEST_HH_ */
