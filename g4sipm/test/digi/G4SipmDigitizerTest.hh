/*
 * G4SipmDigitizerTest.hh
 *
 * @date Mar 19, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMDIGITIZERTEST_HH_
#define G4SIPMDIGITIZERTEST_HH_

#include <gtest/gtest.h>
#include <CLHEP/Random/RandFlat.h>
#include <G4RunManager.hh>
#include <G4DCtable.hh>
#include <G4UImanager.hh>
#include <G4UserRunAction.hh>
#include <G4Run.hh>

#include "G4Sipm.hh"
#include "hit/G4SipmHit.hh"
#include "hit/G4SipmSensitiveDetector.hh"
#include "digi/G4SipmDigi.hh"
#include "digi/G4SipmDigitizer.hh"

#include "sim/TestRun.hh"
#include "sim/TestPrimaryGeneratorAction.hh"
#include "sim/TestEventAction.hh"

class G4SipmDigitizerTest: public testing::Test {
protected:
	TestRun* testRun;

	virtual void SetUp() {
		// Mimick an existing environment. This is important for the G4DigiManager and the G4SDManager to work.
		testRun = new TestRun;
	}

	virtual void TearDown() {
		delete testRun;
		G4RunManager::GetRunManager()->SetUserAction(new TestEventAction());
		G4SipmUiMessenger::getInstance()->setDefaults();
	}

	class DigitizeEventAction: public TestEventAction {
	private:
		double thermalNoiseRate;

	public:
		virtual void EndOfEventAction(const G4Event* event) {
			TestEventAction::EndOfEventAction(event);
			// Get Digi collection.
			G4DigiManager * dm = G4DigiManager::GetDMpointer();
			int collId =
					dm->GetDigiCollectionID(
							((TestDetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction())->getSipm()->getDigiCollectionName());
			const G4SipmDigiCollection* digis = (const G4SipmDigiCollection*) dm->GetDigiCollection(collId);
			// Derive thermal noise rate.
			thermalNoiseRate = 0;
			if (!digis || digis->GetSize() == 0) {
				return;
			}
			std::vector<G4SipmDigi*> v = *digis->GetVector();
			G4SipmDigi* digi = v[0];
			// Get the number and time window of all thermal noise hits.
			double tMin = v.front()->getTime(), tMax = v.back()->getTime();
			unsigned int nThermal = 0;
			for (unsigned int i = 0; i < v.size(); i++) {
				digi = v[i];
				if (digi->getType() == THERMAL) {
					tMin = std::min(tMin, digi->getTime());
					tMax = std::max(tMax, digi->getTime());
					nThermal++;
				}
			}
			thermalNoiseRate = ((double) nThermal) / (tMax - tMin);
		}

		double getThermalNoiseRate() const {
			return thermalNoiseRate;
		}
	};

};

TEST_F(G4SipmDigitizerTest, shouldRegister) {
	ASSERT_TRUE(G4DigiManager::GetDMpointer()->FindDigitizerModule( testRun->getSipm()->getDigitizerName()) != NULL);
}

TEST_F(G4SipmDigitizerTest, thermalNoiseRate) {
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/preThermal 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/postThermal 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/thermal 1");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/afterpulsing 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/crosstalk 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/ifNoSignal 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/digitize/trace 0");
	((TestPrimaryGeneratorAction*) G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction())->setTMax(
			0.1 * CLHEP::second);
	G4SipmDigitizerTest::DigitizeEventAction* eventAction = new DigitizeEventAction();
	G4RunManager::GetRunManager()->SetUserAction(eventAction);
	testRun->beamOn();
	ASSERT_NEAR(testRun->getModel()->getThermalNoiseRate(), eventAction->getThermalNoiseRate(), 0.1 * CLHEP::megahertz);
}

TEST_F(G4SipmDigitizerTest, preThermalNoiseRate) {
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/preThermal 1000");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/postThermal 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/thermal 1");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/afterpulsing 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/crosstalk 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/ifNoSignal 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/digitize/trace 0");
	((TestPrimaryGeneratorAction*) G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction())->setTMax(
			1. * CLHEP::nanosecond);
	G4SipmDigitizerTest::DigitizeEventAction* eventAction = new DigitizeEventAction();
	G4RunManager::GetRunManager()->SetUserAction(eventAction);
	testRun->beamOn();
	ASSERT_NEAR(testRun->getModel()->getThermalNoiseRate(), eventAction->getThermalNoiseRate(), 0.1 * CLHEP::megahertz);
}

TEST_F(G4SipmDigitizerTest, postThermalNoiseRate) {
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/preThermal 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/postThermal 1000");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/thermal 1");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/afterpulsing 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/crosstalk 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/noise/ifNoSignal 0");
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/digitize/trace 0");
	((TestPrimaryGeneratorAction*) G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction())->setTMax(
			1. * CLHEP::nanosecond);
	G4SipmDigitizerTest::DigitizeEventAction* eventAction = new DigitizeEventAction();
	G4RunManager::GetRunManager()->SetUserAction(eventAction);
	testRun->beamOn();
	ASSERT_NEAR(testRun->getModel()->getThermalNoiseRate(), eventAction->getThermalNoiseRate(), 0.1 * CLHEP::megahertz);
}

#endif /* G4SIPMDIGITIZERTEST_HH_ */
