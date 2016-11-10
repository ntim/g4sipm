/*
 * GeneralParticleSource.cc
 *
 * @date Jan 18, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "ParticleSource.hh"

#include <G4ParticleTable.hh>
#include <G4UImanager.hh>
#include <G4RunManager.hh>
#include <G4Run.hh>
#include <G4OpticalPhoton.hh>
#include <Randomize.hh>

#include <CLHEP/Units/SystemOfUnits.h>

#include <boost/any.hpp>
#include <boost/format.hpp>
#include <fstream>
#include <algorithm>

#include "ParticleSourceMessenger.hh"
#include "rnd/Direction.hh"
#include "rnd/Flat.hh"
#include "rnd/General.hh"
#include "rnd/ThreeVector.hh"
#include "rnd/FixedValue.hh"

ParticleSource::ParticleSource() {
	particleGun = new G4ParticleGun(1);
	messenger = ParticleSourceMessenger::getInstance();
}

ParticleSource::~ParticleSource() {
	//
}

G4ThreeVector ParticleSource::getPolarizationVector(double angle, G4ThreeVector kphoton) {
	// Calculate polarization.
	G4ThreeVector normal(1., 0., 0.);
	G4ThreeVector product = normal.cross(kphoton);
	G4double modul2 = product * product;
	G4ThreeVector e_perpend(0., 0., 1.);
	if (modul2 > 0.) {
		e_perpend = (1. / sqrt(modul2)) * product;
	}
	G4ThreeVector e_paralle = e_perpend.cross(kphoton);
	G4ThreeVector polar = cos(angle) * e_paralle + sin(angle) * e_perpend;
	return polar;
}

G4ThreeVector ParticleSource::transformToSourcePlane(G4ThreeVector v) {
	const G4ThreeVector dicingPlaneNormal(0, 0, 1);
	const G4ThreeVector sourcePlaneNormal = messenger->getSurfaceNormal();
	const G4ThreeVector rotationAxis = dicingPlaneNormal.cross(sourcePlaneNormal);
	G4double rotationAngle = 0.;
	if (rotationAxis.mag() != 0.) {
		rotationAngle = acos(
				dicingPlaneNormal * sourcePlaneNormal / (dicingPlaneNormal.mag() * sourcePlaneNormal.mag()));
	} else if (sourcePlaneNormal == -dicingPlaneNormal) {
		rotationAngle = 180. * CLHEP::deg;
	}
	if (rotationAngle == 180. * CLHEP::deg) {
		v *= -1;
	} else if (rotationAngle != 0.) {
		v.rotate(rotationAxis, rotationAngle);
	}
	return v;
}

void ParticleSource::GeneratePrimaries(G4Event* event) {
	const unsigned int nParticles = messenger->getNParticles();
	// Init random number generators.
	rnd::ThreeVector* position = new rnd::ThreeVector(new rnd::Flat(-messenger->getA() / 2., messenger->getA() / 2.),
			new rnd::Flat(-messenger->getB() / 2., messenger->getB() / 2.), new rnd::FixedValue(0.0, 0.0));
	rnd::RandomDouble* polar = new rnd::FixedValue(messenger->getPolar(), 360.0 * CLHEP::deg);
	rnd::RandomDouble* energy =
			messenger->getEInput().empty() ?
					(rnd::RandomDouble*) new rnd::Flat(messenger->getEMin(), messenger->getEMax()) :
					(rnd::RandomDouble*) new rnd::General(messenger->getEInput());
	// Update messenger values.
	if (!messenger->getEInput().empty()) {
		G4UImanager::GetUIpointer()->ApplyCommand(
				boost::str(boost::format("/ps/energy/eMin %g eV") % (((rnd::General*) energy)->getXMin() / CLHEP::eV)));
		G4UImanager::GetUIpointer()->ApplyCommand(
				boost::str(boost::format("/ps/energy/eMax %g eV") % (((rnd::General*) energy)->getXMax() / CLHEP::eV)));
	}
	rnd::RandomDouble* time =
			messenger->getTInput().empty() ?
					(rnd::RandomDouble*) new rnd::Flat(messenger->getTMin(), messenger->getTMax()) :
					(rnd::RandomDouble*) new rnd::General(messenger->getTInput());
	// Update messenger values.
	if (!messenger->getTInput().empty()) {
		G4UImanager::GetUIpointer()->ApplyCommand(
				boost::str(boost::format("/ps/tMin %g ns") % (((rnd::General*) time)->getXMin() / CLHEP::ns)));
		G4UImanager::GetUIpointer()->ApplyCommand(
				boost::str(boost::format("/ps/tMax %g ns") % (((rnd::General*) time)->getXMax() / CLHEP::ns)));
	}
	rnd::ThreeVector* momentum = new rnd::Direction(messenger->getPhiMin(), messenger->getPhiMax(),
			messenger->getThetaMin(), messenger->getThetaMax());
	// Create particles.
	for (size_t i = 0; i < nParticles; i++) {
		particleGun->SetNumberOfParticles(1);
		particleGun->SetParticleDefinition(G4OpticalPhoton::Definition());
		// Dice time.
		particleGun->SetParticleTime(time->shoot());
		// Dice energy.
		particleGun->SetParticleEnergy(energy->shoot());
		// Dice position.
		particleGun->SetParticlePosition(transformToSourcePlane(position->shoot()) + messenger->getPos());
		particleGun->SetParticleMomentumDirection(transformToSourcePlane(momentum->shoot()));
		// Polarization must be computed afterwards.
		particleGun->SetParticlePolarization(
				getPolarizationVector(polar->shoot(), particleGun->GetParticleMomentumDirection()));
		// Fire!
		particleGun->GeneratePrimaryVertex(event);
	}
	// Cleanup
	delete position;
	delete time;
	delete energy;
	delete polar;
	delete momentum;
}

