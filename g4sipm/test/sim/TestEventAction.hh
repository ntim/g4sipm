/*
 * TestEventAction.hh
 *
 * @date Mar 26, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef TESTEVENTACTION_HH_
#define TESTEVENTACTION_HH_

#include <G4UserEventAction.hh>
#include <G4DigiManager.hh>
#include <G4DCtable.hh>
#include <G4VDigitizerModule.hh>
#include <G4TDigiCollection.hh>

class TestEventAction: public G4UserEventAction {
public:
	virtual void EndOfEventAction(const G4Event* event) {
		G4UserEventAction::EndOfEventAction(event);
		G4DigiManager* digiManager = G4DigiManager::GetDMpointer();
		G4DCtable* dcTable = digiManager->GetDCtable();
		for (int i = 0; i < dcTable->entries(); i++) {
			G4String dmName = dcTable->GetDMname(i);
			std::cout << "TestEventAction::EndOfEventAction(): Attempt to digitize " << dmName << "." << std::endl;
			G4VDigitizerModule* dm = digiManager->FindDigitizerModule(dmName);
			if (dm) {
				dm->Digitize();
			}
		}
	}
};

#endif /* TESTEVENTACTION_HH_ */
