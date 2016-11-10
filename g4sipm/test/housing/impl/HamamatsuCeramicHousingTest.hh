/*
 * HamamatsuCeramicHousingTest.hh
 *
 * @created Apr 29, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUCERAMICHOUSINGTEST_HH_
#define HAMAMATSUCERAMICHOUSINGTEST_HH_

#include <gtest/gtest.h>

#include <G4RunManager.hh>

#include "housing/impl/HamamatsuCeramicHousing.hh"

class HamamatsuCeramicHousingTest: public testing::Test {
protected:
	G4SipmHousing* testedObject;

	virtual void SetUp() {
		testedObject = new HamamatsuCeramicHousing(new G4Sipm(new G4SipmGenericSipmModel));
	}

	virtual void TearDown() {
		delete testedObject;
		G4RunManager::GetRunManager()->GeometryHasBeenModified();
	}
};

TEST_F(HamamatsuCeramicHousingTest, build) {
	testedObject->build();
}

#endif /* HAMAMATSUCERAMICHOUSINGTEST_HH_ */
