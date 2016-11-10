/*
 * G4SipmConfigFileModelTest.hh
 *
 * @date Mar 16, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMCONFIGFILEMODELTEST_HH_
#define G4SIPMCONFIGFILEMODELTEST_HH_

#include <gtest/gtest.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "model/impl/G4SipmConfigFileModel.hh"

class G4SipmConfigFileModelTest: public testing::Test {
protected:
	G4SipmModel* testedObject;

	virtual void SetUp() {
		testedObject = new G4SipmConfigFileModel("resources/hamamatsu-s10362-11-100c.properties");
		testedObject->setBiasVoltage(73. * CLHEP::volt);
	}
};

TEST_F(G4SipmConfigFileModelTest, updateOperatingPoint) {
	testedObject->setBiasVoltage((71.5 + 0.8) * CLHEP::volt);
	ASSERT_NEAR(494000 * CLHEP::hertz, testedObject->getThermalNoiseRate(), 1.0 * CLHEP::hertz);
	testedObject->setBiasVoltage((71.5 + 1.5) * CLHEP::volt);
	ASSERT_NEAR(924000 * CLHEP::hertz, testedObject->getThermalNoiseRate(), 1.0 * CLHEP::hertz);
	// Test interpolation in between.
	testedObject->setBiasVoltage((71.5 + 0.85) * CLHEP::volt);
	double v[] = { (71.5 + 0.8) * CLHEP::volt, (71.5 + 0.9) * CLHEP::volt };
	double q[] = { 494000 * CLHEP::hertz, 508000 * CLHEP::hertz };
	double fth = InterpolationUtil::linear(std::vector<double>(v, v + 2), std::vector<double>(q, q + 2),
			(71.5 + 0.85) * CLHEP::volt);
	ASSERT_NEAR(fth, testedObject->getThermalNoiseRate(), 1.0 * CLHEP::hertz);
}

TEST_F(G4SipmConfigFileModelTest, shouldFailToCreate) {
	EXPECT_ANY_THROW({
		new G4SipmConfigFileModel("does-not-exist.properties");
	});
}

TEST_F(G4SipmConfigFileModelTest, getName) {
	ASSERT_STREQ("Hamamatsu_S10362-11-100C", testedObject->getName().c_str());
}

TEST_F(G4SipmConfigFileModelTest, getTemperature) {
	ASSERT_DOUBLE_EQ(20.0 * CLHEP::kelvin + CLHEP::STP_Temperature, testedObject->getTemperature());
}

TEST_F(G4SipmConfigFileModelTest, getThickness) {
	ASSERT_DOUBLE_EQ(0.1 * CLHEP::mm, testedObject->getThickness());
}

TEST_F(G4SipmConfigFileModelTest, getNumberOfCells) {
	ASSERT_DOUBLE_EQ(100, testedObject->getNumberOfCells());
}

TEST_F(G4SipmConfigFileModelTest, getCellPitch) {
		ASSERT_DOUBLE_EQ(0.1 * CLHEP::mm, testedObject->getCellPitch());
}

TEST_F(G4SipmConfigFileModelTest, getFillFactor) {
	ASSERT_DOUBLE_EQ(0.785, testedObject->getFillFactor());
}

TEST_F(G4SipmConfigFileModelTest, getBreakdownVoltage) {
	ASSERT_DOUBLE_EQ(71.5 * CLHEP::volt, testedObject->getBreakdownVoltage());
}

TEST_F(G4SipmConfigFileModelTest, getBiasVoltage) {
	ASSERT_DOUBLE_EQ(73.0 * CLHEP::volt, testedObject->getBiasVoltage());
}

TEST_F(G4SipmConfigFileModelTest, getPhotonDetectionEfficiency) {
	ASSERT_DOUBLE_EQ(0.0918187959596, testedObject->getPhotonDetectionEfficiency(319.49 * CLHEP::nm));
	ASSERT_DOUBLE_EQ(0.149970876768, testedObject->getPhotonDetectionEfficiency(327.842 * CLHEP::nm));
	// Test linear interpolation.
	ASSERT_NEAR(0.09406152880528329, testedObject->getPhotonDetectionEfficiency(320.0 * CLHEP::nm), 1e-5);
}

TEST_F(G4SipmConfigFileModelTest, getThermalNoiseRate) {
	ASSERT_DOUBLE_EQ(0.924000 * CLHEP::megahertz, testedObject->getThermalNoiseRate());
}

TEST_F(G4SipmConfigFileModelTest, getCrossTalkProbability) {
	ASSERT_DOUBLE_EQ(19.3662 * CLHEP::perCent, testedObject->getCrossTalkProbability());
}

TEST_F(G4SipmConfigFileModelTest, getAfterPulseProbLong) {
	ASSERT_DOUBLE_EQ(26.01786 * CLHEP::perCent, testedObject->getApProbLong());
}

TEST_F(G4SipmConfigFileModelTest, getAfterPulseProbShort) {
	ASSERT_DOUBLE_EQ(21.28734 * CLHEP::perCent, testedObject->getApProbShort());
}

TEST_F(G4SipmConfigFileModelTest, getAfterPulseTauLong) {
	ASSERT_DOUBLE_EQ(123.1 * CLHEP::ns, testedObject->getApTauLong());
}

TEST_F(G4SipmConfigFileModelTest, getAfterPulseTauShort) {
	ASSERT_DOUBLE_EQ(44.5 * CLHEP::ns, testedObject->getApTauShort());
}

TEST_F(G4SipmConfigFileModelTest, getRecoveryTime) {
	ASSERT_DOUBLE_EQ(41.1 * CLHEP::ns, testedObject->getRecoveryTime());
}

TEST_F(G4SipmConfigFileModelTest, shouldInterpolate) {
	testedObject->setBiasVoltage(72.95 * CLHEP::volt);
	ASSERT_DOUBLE_EQ(0.940 * CLHEP::megahertz, testedObject->getThermalNoiseRate());
}

TEST_F(G4SipmConfigFileModelTest, getDeadTime) {
	ASSERT_DOUBLE_EQ(3.0 * CLHEP::picosecond, testedObject->getDeadTime());
}

TEST_F(G4SipmConfigFileModelTest, getGainVariation) {
	ASSERT_DOUBLE_EQ(1.0 * CLHEP::perCent, testedObject->getGainVariation());
}

TEST_F(G4SipmConfigFileModelTest, getGain) {
	for (G4SipmCellId id = 0; id < testedObject->getNumberOfCells(); id++) {
		ASSERT_TRUE(testedObject->getGain(id));
	}
}

#endif /* G4SIPMCONFIGFILEMODELTEST_HH_ */
