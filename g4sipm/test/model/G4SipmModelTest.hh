/*
 * G4SipmModelTest.hh
 *
 * @date 25.02.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMMODELTEST_HH_
#define G4SIPMMODELTEST_HH_

#include <gtest/gtest.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "model/G4SipmModel.hh"

class G4SipmModelTest: public testing::Test {
protected:
	const G4SipmModel* testedObject;

	virtual void SetUp() {
		testedObject = new G4SipmGenericSipmModel;
	}
};

TEST_F(G4SipmModelTest, getPitch) {
	ASSERT_DOUBLE_EQ(1. * CLHEP::mm, testedObject->getPitch());
}

TEST_F(G4SipmModelTest, getOvervoltage) {
	ASSERT_DOUBLE_EQ(testedObject->getBiasVoltage() - testedObject->getBreakdownVoltage(),
			testedObject->getOverVoltage());
}

TEST_F(G4SipmModelTest, getCellId) {
	double pos = -testedObject->getPitch() / 2.;
	ASSERT_EQ((G4SipmCellId) 0, testedObject->getCellId(pos, pos, false));
}

TEST_F(G4SipmModelTest, isValidCellId) {
	double pos = -testedObject->getPitch() / 2.;
	ASSERT_TRUE(testedObject->isValidCellId(testedObject->getCellId(pos, pos, false)));
	ASSERT_FALSE(testedObject->isValidCellId(testedObject->getCellId(pos, pos, true)));
}

TEST_F(G4SipmModelTest, getCellPosition) {
	CLHEP::Hep2Vector pos = testedObject->getCellPosition(0);
	double expected = -testedObject->getPitch() / 2. + testedObject->getCellPitch() / 2.;
	ASSERT_DOUBLE_EQ(expected, pos.x());
	ASSERT_DOUBLE_EQ(expected, pos.y());
}

TEST_F(G4SipmModelTest, getCellPositionCellId) {
	for (unsigned int i = 0; i < testedObject->getNumberOfCells(); i++) {
		CLHEP::Hep2Vector pos = testedObject->getCellPosition(i);
		ASSERT_EQ(i, testedObject->getCellId(pos.x(), pos.y(), false));
		ASSERT_EQ(i, testedObject->getCellId(pos.x(), pos.y(), true));
	}
}

TEST_F(G4SipmModelTest, getGain) {
	for (unsigned int i = 0; i < testedObject->getNumberOfCells(); i++) {
		ASSERT_TRUE(testedObject->getGain(i) > 0.);
	}
}

#endif /* G4SIPMMODELTEST_HH_ */
