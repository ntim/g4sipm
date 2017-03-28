/*
 * OpticalPhysicsList.cc
 *
 * @date Sep 12, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "OpticalPhysicsList.hh"

#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4Proton.hh>

OpticalPhysicsList::OpticalPhysicsList(int verbose, std::vector<G4OpticalProcessIndex> deactivate) {
	G4OpticalPhysics* phys = new G4OpticalPhysics(verbose);
	// Deactivate processes
	for (std::vector<G4OpticalProcessIndex>::iterator it = deactivate.begin(); it != deactivate.end(); it++) {
		phys->Configure(*it, false);
	}
	//
	RegisterPhysics(phys);
	SetVerboseLevel(verbose);
}

OpticalPhysicsList::~OpticalPhysicsList() {
	//
}

void OpticalPhysicsList::ConstructParticle() {
	G4VModularPhysicsList::ConstructParticle();
	G4Gamma::GammaDefinition();
	G4Electron::ElectronDefinition();
	G4Positron::PositronDefinition();
	G4Proton::ProtonDefinition();
}

void OpticalPhysicsList::SetCuts() {
	SetCutsWithDefault();
	if (this->verboseLevel > 0) {
		DumpCutValuesTable();
	}
}
