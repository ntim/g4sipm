/*
 * TestPrimaryGeneratorAction.hh
 *
 * @date Mar 23, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef TESTPRIMARYGENERATORACTION_HH_
#define TESTPRIMARYGENERATORACTION_HH_

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleDefinition.hh>
#include <G4OpticalPhoton.hh>
#include <G4ThreeVector.hh>
#include <CLHEP/Units/SystemOfUnits.h>

#include "model/G4SipmModel.hh"

class TestPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction {
private:
	G4ParticleGun* particleGun;
	unsigned int nParticles;
	unsigned int nVertices;
	double tMax;
	G4SipmModel* model;

public:
	TestPrimaryGeneratorAction(G4SipmModel* _model) :
			G4VUserPrimaryGeneratorAction(), nParticles(100), nVertices(100), tMax(1000 * CLHEP::nanosecond), model(
					_model) {
		particleGun = new G4ParticleGun(nParticles);
		// Initialize.
		G4ParticleDefinition* particle = G4OpticalPhoton::Definition();
		particleGun->SetParticleDefinition(particle);
		particleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, -1.));
		particleGun->SetParticleEnergy(3. * CLHEP::eV);
		particleGun->SetParticlePolarization(G4ThreeVector(0, 0, 1.));
	}

	virtual void GeneratePrimaries(G4Event* event) {
		const double dx = model->getPitch() / 2.;
		for (unsigned int i = 0; i < nVertices; i++) {
			particleGun->SetParticlePosition(
					G4ThreeVector(CLHEP::RandFlat::shoot(-dx, dx), CLHEP::RandFlat::shoot(-dx, dx), 1.0 * CLHEP::mm));
			particleGun->SetParticleTime(CLHEP::RandFlat::shoot(tMax));
			particleGun->GeneratePrimaryVertex(event);
		}
	}

	unsigned int getNParticles() const {
		return nParticles;
	}

	void setNParticles(unsigned int _nParticles) {
		nParticles = _nParticles;
	}

	unsigned int getVertices() const {
		return nVertices;
	}

	void setVertices(unsigned int _nVertices) {
		nVertices = _nVertices;
	}

	double getTMax() const {
		return tMax;
	}

	void setTMax(double _tMax) {
		tMax = _tMax;
	}
};

#endif /* TESTPRIMARYGENERATORACTION_HH_ */
