/*
 * AsciiPersistencyHandlerMessenger.cc
 *
 * @date 24.08.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "persistency/PersistencyHandlerMessenger.hh"

#include <G4UIdirectory.hh>
#include <CLHEP/Units/SystemOfUnits.h>

#include "G4UiMessengerUtil.hh"
#include "ParticleSourceMessenger.hh"

PersistencyHandlerMessenger::PersistencyHandlerMessenger() {
	new G4UIdirectory("/g4sipm/persist/");
	// Set defaults.
	tMin = ParticleSourceMessenger::getInstance()->getTMin() - 1000. * CLHEP::ns;
	tMax = ParticleSourceMessenger::getInstance()->getTMax() + 1000. * CLHEP::ns;
	persistHits = true;
	persistDigis = true;
	// Initialize commands.
	tMinCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/persist/", "tMin", tMin, "s");
	tMaxCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/persist/", "tMax", tMax, "s");
	persistHitsCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/persist/", "hits", persistHits);
	persistDigisCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/persist/", "digis", persistDigis);
}

PersistencyHandlerMessenger::~PersistencyHandlerMessenger() {
	delete tMinCmd;
	delete tMaxCmd;
	delete persistHitsCmd;
	delete persistDigisCmd;
}

PersistencyHandlerMessenger* PersistencyHandlerMessenger::getInstance() {
	static PersistencyHandlerMessenger* instance = new PersistencyHandlerMessenger;
	return instance;
}

void PersistencyHandlerMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
	G4UiMessengerUtil::setNewValueIfCmdMatches(command, tMinCmd, newValue, &tMin);
	G4UiMessengerUtil::setNewValueIfCmdMatches(command, tMaxCmd, newValue, &tMax);
	G4UiMessengerUtil::setNewValueIfCmdMatches(command, persistHitsCmd, newValue, &persistHits);
	G4UiMessengerUtil::setNewValueIfCmdMatches(command, persistDigisCmd, newValue, &persistDigis);
}

double PersistencyHandlerMessenger::getTMax() const {
	return tMax;
}

double PersistencyHandlerMessenger::getTMin() const {
	return tMin;
}

bool PersistencyHandlerMessenger::isPersistHits() const {
	return persistHits;
}

bool PersistencyHandlerMessenger::isPersistDigis() const {
	return persistDigis;
}
