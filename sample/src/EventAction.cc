/*
 * EventAction.cc
 *
 * @date Feb 23, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "EventAction.hh"

#include <G4RunManager.hh>
#include <G4DigiManager.hh>

#include "RunAction.hh"
#include "persistency/PersistencyHandler.hh"

EventAction::EventAction() {
	//
}

EventAction::~EventAction() {
	//
}

void EventAction::EndOfEventAction(const G4Event* event) {
	PersistencyHandler* persistency =
			((RunAction*) G4RunManager::GetRunManager()->GetUserRunAction())->getPersistencyHandler();
	// Run all digitizer modules.
	G4DigiManager* digiManager = G4DigiManager::GetDMpointer();
	G4DCtable* dcTable = digiManager->GetDCtable();
	for (int i = 0; i < dcTable->entries(); i++) {
		G4String dmName = dcTable->GetDMname(i);
		G4VDigitizerModule* dm = digiManager->FindDigitizerModule(dmName);
		if (dm) {
			dm->Digitize();
		}
	}
	G4Timer timer;
	timer.Start();
	// Process hits collections.
	G4HCofThisEvent* hCof = event->GetHCofThisEvent();
	if (hCof != NULL) {
		for (int i = 0; i < hCof->GetCapacity(); ++i) {
			G4VHitsCollection* hc = hCof->GetHC(i);
			if (hc != NULL) {
				if (dynamic_cast<G4SipmHitsCollection*>(hc)) {
					persistency->persist((G4SipmHitsCollection*) hc);
				}
			}
		}
	}
	// Process digi collections.
	G4DCofThisEvent* dCof = event->GetDCofThisEvent();
	if (dCof != NULL) {
		for (int i = 0; i < dCof->GetCapacity(); ++i) {
			G4VDigiCollection* dc = dCof->GetDC(i);
			if (dc != NULL) {
				if (dynamic_cast<G4SipmDigiCollection*>(dc)) {
					persistency->persist((G4SipmDigiCollection*) dc);
				}
				if (dynamic_cast<G4SipmVoltageTraceDigiCollection*>(dc)) {
					persistency->persist((G4SipmVoltageTraceDigiCollection*) dc);
				}
			}
		}
	}
	timer.Stop();
	std::cout << "EventAction::EndOfEventAction(): persist time (" << timer << ")." << std::endl;
}
