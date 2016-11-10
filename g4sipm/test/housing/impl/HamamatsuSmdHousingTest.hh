/*
 * HamamatsuSmdHousingTest.hh
 *
 * @created Apr 29, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUSMDHOUSINGTEST_HH_
#define HAMAMATSUSMDHOUSINGTEST_HH_

#include <gtest/gtest.h>

#include <G4RunManager.hh>

#include "housing/impl/HamamatsuSmdHousing.hh"

class HamamatsuSmdHousingTest: public testing::Test {
protected:
	G4SipmHousing* testedObject;

	virtual void SetUp() {
		testedObject = new HamamatsuSmdHousing(new G4Sipm(new G4SipmGenericSipmModel));
	}

	virtual void TearDown() {
		delete testedObject;
		G4RunManager::GetRunManager()->GeometryHasBeenModified();
	}
};

TEST_F(HamamatsuSmdHousingTest, build) {
	testedObject->build();
}

#endif /* HAMAMATSUSMDHOUSINGTEST_HH_ */
