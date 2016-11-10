/*
 * G4SipmTest.hh
 *
 * @date 26.02.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMTEST_HH_
#define G4SIPMTEST_HH_

#include <gtest/gtest.h>
#include <set>
#include <G4SDManager.hh>
#include <G4DigiManager.hh>
#include <G4RunManager.hh>

#include "sim/TestRun.hh"
#include "G4Sipm.hh"
#include "model/impl/G4SipmGenericSipmModel.hh"

class G4SipmTest: public testing::Test {
protected:
	TestRun* testRun;

	virtual void SetUp() {
		// Mimick an existing environment.
		// This is important for the G4DigiManager and the G4SDManager to work.
		testRun = new TestRun();
	}

	virtual void TearDown() {
		delete testRun;
	}
};

TEST_F(G4SipmTest, getId) {
	std::set<int> ids;
	const unsigned int nTrial = 10;
	for (unsigned int i = 0; i < nTrial; i++) {
		G4Sipm* to = new G4Sipm(new G4SipmGenericSipmModel);
		ids.insert(to->getId());
		delete to;
	}
	ASSERT_EQ(nTrial, ids.size());
	ASSERT_FALSE(ids.count(testRun->getSipm()->getId()));
}

TEST_F(G4SipmTest, shouldRegisterSensitiveDetector) {
	G4String sdName = testRun->getSipm()->getSensitiveDetectorName();
	ASSERT_TRUE(G4SDManager::GetSDMpointer()->FindSensitiveDetector(sdName) != NULL);
}

TEST_F(G4SipmTest, shouldRegisterDigitizerModule) {
	G4String dmName = testRun->getSipm()->getDigitizerName();
	ASSERT_TRUE(G4DigiManager::GetDMpointer()->FindDigitizerModule(dmName) != NULL);
}

TEST_F(G4SipmTest, shouldRegisterVoltageTraceDigitizerModule) {
	G4String dmName = testRun->getSipm()->getVoltageTraceDigitizerName();
	ASSERT_TRUE(G4DigiManager::GetDMpointer()->FindDigitizerModule(dmName) != NULL);
}

#endif /* G4SIPMTEST_HH_ */
