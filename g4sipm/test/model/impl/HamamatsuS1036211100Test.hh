/*
 * HamamatsuS1036211100Test.hh
 *
 * @created Apr 25, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUS1036211100TEST_HH_
#define HAMAMATSUS1036211100TEST_HH_

#include <gtest/gtest.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "model/impl/HamamatsuS1036211100.hh"

class HamamatsuS1036211100Test: public testing::Test {
protected:
	const G4SipmModel* testedObject;

	virtual void SetUp() {
		testedObject = new HamamatsuS1036211100;
	}
};

TEST_F(HamamatsuS1036211100Test, getDeadTime) {
	ASSERT_NEAR(2.84551 * CLHEP::picosecond, testedObject->getDeadTime(), 0.1 * CLHEP::picosecond);
}

TEST_F(HamamatsuS1036211100Test, getGain) {
	for (G4SipmCellId id = 0; id < testedObject->getNumberOfCells(); id++) {
		ASSERT_TRUE(testedObject->getGain(id));
	}
}

TEST_F(HamamatsuS1036211100Test, getPhotonDetectionEfficiency) {
	ASSERT_NEAR(25.307577083333327 * CLHEP::perCent, testedObject->getPhotonDetectionEfficiency(350. * CLHEP::nanometer), 0.1 * CLHEP::perCent);
}

#endif /* HAMAMATSUS1036211100TEST_HH_ */
