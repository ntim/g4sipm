/*
 * FastParticleSource.hh
 *
 * @date Jun 3, 2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FASTPARTICLESOURCE_HH_
#define FASTPARTICLESOURCE_HH_

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4Geantino.hh>
#include <G4ParticleGun.hh>
#include <G4Event.hh>

#include "DetectorConstruction.hh"
#include "housing/G4SipmHousing.hh"

class FastParticleSource: public G4VUserPrimaryGeneratorAction {
private:
	G4ParticleGun* gun;

public:
	FastParticleSource() {
		gun = new G4ParticleGun(G4Geantino::Definition());
	}

	virtual void GeneratePrimaries(G4Event* event) {
		gun->GeneratePrimaryVertex(event);
	}
};

#endif /* FASTPARTICLESOURCE_HH_ */
