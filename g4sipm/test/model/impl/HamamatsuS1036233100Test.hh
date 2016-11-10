/*
 * HamamatsuS1036233100Test.hh
 *
 * @created Apr 30, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUS1036233100TEST_HH_
#define HAMAMATSUS1036233100TEST_HH_

#include <gtest/gtest.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "model/impl/HamamatsuS1036233100.hh"

class HamamatsuS1036233100Test: public testing::Test {
protected:
	const G4SipmModel* testedObject;

	virtual void SetUp() {
		testedObject = new HamamatsuS1036233100;
	}
};

TEST_F(HamamatsuS1036233100Test, getNumberOfCells) {
	ASSERT_EQ((unsigned int ) 900, testedObject->getNumberOfCells());
}

TEST_F(HamamatsuS1036233100Test, getPitch) {
	ASSERT_EQ(3.0 * CLHEP::mm, testedObject->getPitch());
}

TEST_F(HamamatsuS1036233100Test, getPhotonDetectionEfficiency) {
	ASSERT_NEAR(25.307577083333327 * CLHEP::perCent,
			testedObject->getPhotonDetectionEfficiency(350.0 * CLHEP::nanometer), 0.1 * CLHEP::perCent);
}

#endif /* HAMAMATSUS1036233100TEST_HH_ */
