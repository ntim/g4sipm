/*
 * G4SipmConfigFileVoltageTraceModelTest.hh
 *
 * @created Apr 29, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMCONFIGFILEVOLTAGETRACEMODELTEST_HH_
#define G4SIPMCONFIGFILEVOLTAGETRACEMODELTEST_HH_

#include <gtest/gtest.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "model/impl/G4SipmConfigFileVoltageTraceModel.hh"

class G4SipmConfigFileVoltageTraceModelTest: public testing::Test {
protected:
	G4SipmConfigFileVoltageTraceModel* testedObject;

	virtual void SetUp() {
		testedObject = new G4SipmConfigFileVoltageTraceModel("resources/hamamatsu-s10362-11-100c.properties");
	}
};

TEST_F(G4SipmConfigFileVoltageTraceModelTest, getAmplitude) {
	ASSERT_EQ(14.1e-3 * CLHEP::volt, testedObject->getAmplitude());
}

TEST_F(G4SipmConfigFileVoltageTraceModelTest, getTauRise) {
	ASSERT_EQ(8.1 * CLHEP::nanosecond, testedObject->getTauRise());
}

TEST_F(G4SipmConfigFileVoltageTraceModelTest, getTauFall) {
	ASSERT_EQ(43.6 * CLHEP::nanosecond, testedObject->getTauFall());
}

TEST_F(G4SipmConfigFileVoltageTraceModelTest, getV0) {
	ASSERT_EQ(4.7e-3 * CLHEP::volt, testedObject->getV0());
}

TEST_F(G4SipmConfigFileVoltageTraceModelTest, getWhiteNoiseSigma) {
	ASSERT_EQ(1e-3 * CLHEP::volt, testedObject->getWhiteNoiseSigma());
}

TEST_F(G4SipmConfigFileVoltageTraceModelTest, getTimeBinWidth) {
	ASSERT_EQ(1.0 * CLHEP::nanosecond, testedObject->getTimeBinWidth());
}

TEST_F(G4SipmConfigFileVoltageTraceModelTest, getPrecision) {
	ASSERT_EQ(12, testedObject->getPrecision());
}

#endif /* G4SIPMCONFIGFILEVOLTAGETRACEMODELTEST_HH_ */
