/*
 * G4SipmSensitiveDetectorTest.hh
 *
 * @date 25.02.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMSENSITIVEDETECTORTEST_HH_
#define G4SIPMSENSITIVEDETECTORTEST_HH_

#include <gtest/gtest.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4SDManager.hh>
#include <G4HCtable.hh>
#include <G4RunManager.hh>

#include "G4Sipm.hh"
#include "hit/G4SipmSensitiveDetector.hh"
#include "sim/TestRun.hh"

class G4SipmSensitiveDetectorTest: public testing::Test {
protected:
	TestRun* testRun;
	G4Sipm* sipm;
	G4SipmSensitiveDetector* testedObject;

	virtual void SetUp() {
		testRun = new TestRun;
		sipm = testRun->getSipm();
		testedObject = (G4SipmSensitiveDetector*) G4SDManager::GetSDMpointer()->FindSensitiveDetector(
				sipm->getSensitiveDetectorName());
	}

	virtual void TearDown() {
		delete testRun;
	}
};

TEST_F(G4SipmSensitiveDetectorTest, shouldRegister) {
	G4HCtable* table = G4SDManager::GetSDMpointer()->GetHCtable();
	const int collId = table->GetCollectionID(testedObject);
	ASSERT_TRUE(collId >= 0);
	G4String hitCollName = table->GetHCname(collId);
	G4String sdName = table->GetSDname(collId);
	ASSERT_STREQ(sipm->getHitCollectionName().data(), hitCollName.data());
	ASSERT_STREQ(sipm->getSensitiveDetectorName().data(), sdName.data());
}

TEST_F(G4SipmSensitiveDetectorTest, shouldSetFilter) {
	ASSERT_TRUE(testedObject->GetFilter());
}

#endif /* G4SIPMSENSITIVEDETECTORTEST_HH_ */
