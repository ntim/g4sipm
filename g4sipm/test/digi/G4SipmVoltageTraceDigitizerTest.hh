/*
 * G4SipmVoltageTraceDigitizerTest.hh
 *
 * @date Mar 19, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMVOLTAGETRACEDIGITIZERTEST_HH_
#define G4SIPMVOLTAGETRACEDIGITIZERTEST_HH_

#include <gtest/gtest.h>
#include <CLHEP/Random/RandFlat.h>
#include <G4RunManager.hh>
#include <G4DCtable.hh>

#include "digi/G4SipmVoltageTraceDigitizer.hh"
#include "G4Sipm.hh"
#include "hit/G4SipmHit.hh"
#include "hit/G4SipmSensitiveDetector.hh"

#include "sim/TestRun.hh"

class G4SipmVoltageTraceDigitizerTest: public testing::Test {
protected:
	TestRun* testRun;
	G4SipmVoltageTraceDigitizer* testedObject;

	virtual void SetUp() {
		// Mimick an existing environment. This is important for the G4DigiManager and the G4SDManager to work.
		testRun = new TestRun;
		testedObject = (G4SipmVoltageTraceDigitizer*) G4DigiManager::GetDMpointer()->FindDigitizerModule(
				testRun->getSipm()->getVoltageTraceDigitizerName());
	}

	virtual void TearDown() {
		delete testRun;
	}
};

TEST_F(G4SipmVoltageTraceDigitizerTest, shouldRegister) {
	ASSERT_TRUE(testedObject != NULL);
}

TEST_F(G4SipmVoltageTraceDigitizerTest, digitize) {
	testRun->beamOn();
	// Get digi collection.
//	int colId = G4DigiManager::GetDMpointer()->GetDigiCollectionID(testedObject->GetCollectionName(0));
//	ASSERT_GE(colId, 0);
//	const G4VDigiCollection* digis = testRun->getLastProcessedEvent()->GetDCofThisEvent()->GetDC(colId);
//	ASSERT_TRUE(digis != NULL);
	// FIXME: the container of the digi collection is null.
}

#endif /* G4SIPMVOLTAGETRACEDIGITIZERTEST_HH_ */
