/*
 * RunAction.cc
 *
 * @date 26.02.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "RunAction.hh"

#include <G4RunManager.hh>

#include "G4SipmUiMessenger.hh"
#include "ParticleSourceMessenger.hh"
#include "DetectorConstruction.hh"
#include "persistency/PersistencyHandler.hh"
#include "persistency/PersistVisitorFactory.hh"

RunAction::RunAction(std::string _filename) :
		G4UserRunAction(), filename(_filename) {
	persistencyHandler = new PersistencyHandler(PersistVisitorFactory::getInstance()->create(filename));
}

RunAction::~RunAction() {
}

void RunAction::BeginOfRunAction(const G4Run*) {
	timer.Start();
	persistencyHandler->open(filename);
}

void RunAction::EndOfRunAction(const G4Run*) {
	// Persist run settings.
	persistencyHandler->persist(G4SipmUiMessenger::getInstance());
	persistencyHandler->persist(ParticleSourceMessenger::getInstance());
	// Get detector.
	const DetectorConstruction* detector =
			(const DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	persistencyHandler->persist(detector->getSipmHousing()->getSipm()->getModel());
	persistencyHandler->persist(detector->getSipmHousing()->getSipm()->getModel()->getVoltageTraceModel());
	// Close output.
	persistencyHandler->close();
	timer.Stop();
	std::cout << "RunAction::EndOfRunAction(): total run time (" << timer << ")." << std::endl;
}

std::string RunAction::getFilename() const {
	return filename;
}

PersistencyHandler* RunAction::getPersistencyHandler() const {
	return persistencyHandler;
}
