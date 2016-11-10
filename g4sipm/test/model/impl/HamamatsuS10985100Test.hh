/*
 * HamamatsuS10985100Test.hh
 *
 * @created Apr 30, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUS10985100TEST_HH_
#define HAMAMATSUS10985100TEST_HH_

#include <gtest/gtest.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "model/impl/HamamatsuS10985100.hh"

class HamamatsuS10985100Test: public testing::Test {
protected:
	const G4SipmModel* testedObject;

	virtual void SetUp() {
		testedObject = new HamamatsuS10985100;
	}
};

TEST_F(HamamatsuS10985100Test, getNumberOfCells) {
	ASSERT_EQ((unsigned int) 3600, testedObject->getNumberOfCells());
}

TEST_F(HamamatsuS10985100Test, getPitch) {
	ASSERT_EQ(6.0 * CLHEP::mm, testedObject->getPitch());
}

TEST_F(HamamatsuS10985100Test, getPhotonDetectionEfficiency) {
	ASSERT_NEAR(25.307577083333327 * CLHEP::perCent, testedObject->getPhotonDetectionEfficiency(350. * CLHEP::nanometer), 0.1 * CLHEP::perCent);
}

#endif /* HAMAMATSUS10985100TEST_HH_ */
