/*
 * TestRun.hh
 *
 * @date Mar 23, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef TESTRUN_HH_
#define TESTRUN_HH_

#include <gtest/gtest.h>

#include <G4RunManager.hh>

#include "TestDetectorConstruction.hh"
#include "OpticalPhysicsList.hh"
#include "TestActionInitialization.hh"
#include "model/impl/G4SipmGenericSipmModel.hh"

class TestRun {
private:
	G4RunManager* runManager;

public:
	TestRun() {
		runManager = G4RunManager::GetRunManager();
		if (!runManager) {
			// Construct the default run manager.
			runManager = new G4RunManager();
			// Create SiPM model.
			G4SipmModel* model = new G4SipmGenericSipmModel;
			// Set mandatory initialization classes
			runManager->SetUserInitialization(new TestDetectorConstruction(model));
			runManager->SetUserInitialization(new OpticalPhysicsList);
			runManager->SetUserInitialization(new TestActionInitialization(model));
			// Initialize G4 kernel.
			runManager->Initialize();
		}
	}

	void beamOn() {
		runManager->SetNumberOfEventsToBeStored(100);
		runManager->BeamOn(1);
	}

	G4SipmModel* getModel() const {
		return getSipm()->getModel();
	}

	G4Sipm* getSipm() const {
		return ((TestDetectorConstruction*) runManager->GetUserDetectorConstruction())->getSipm();
	}

};

#endif /* TESTRUN_HH_ */
