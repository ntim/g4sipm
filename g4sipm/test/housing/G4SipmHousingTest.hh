/*
 * G4SipmHousingTest.hh
 *
 * @created Apr 25, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMHOUSINGTEST_HH_
#define G4SIPMHOUSINGTEST_HH_

#include <gtest/gtest.h>

#include <G4RunManager.hh>

#include "housing/G4SipmHousing.hh"

class G4SipmHousingTest: public testing::Test {
protected:
	G4SipmHousing* testedObject;

	virtual void SetUp()  {
		testedObject = new G4SipmHousing(new G4Sipm(new G4SipmGenericSipmModel));
	}

	virtual void TearDown() {
		delete testedObject;
		G4RunManager::GetRunManager()->GeometryHasBeenModified();
	}
};

TEST_F(G4SipmHousingTest, build) {
	testedObject->build();
}

#endif /* G4SIPMHOUSINGTEST_HH_ */
