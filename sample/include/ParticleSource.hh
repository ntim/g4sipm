/*
 * ParticleSource.hh
 *
 * @date Jan 18, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef PARTICLESOURCE_HH_
#define PARTICLESOURCE_HH_

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4Event.hh>
#include <G4ParticleGun.hh>

#include "ParticleSourceMessenger.hh"

/**
 * The general particle source follows the example of its Geant4 pendant. The type, energy- and
 * angular distribution of the particles shoot by the source can be manipulated via the ParticleSourceMessenger
 * class.
 */
class ParticleSource: public G4VUserPrimaryGeneratorAction {
private:
	G4ParticleGun* particleGun;
	ParticleSourceMessenger* messenger;
	/**
	 * Transforms a position or momentum direction from a plane with normal (0,0,-1) to
	 * to the surface plane of the source.
	 */
	G4ThreeVector transformToSourcePlane(G4ThreeVector v);
	G4ThreeVector getPolarizationVector(double angle, G4ThreeVector kphoton);

public:
	/**
	 * Constructor.
	 */
	ParticleSource();
	~ParticleSource();

	virtual void GeneratePrimaries(G4Event* event);
};

#endif /* PARTICLESOURCE_HH_ */
