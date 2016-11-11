/*
 * FastEventAction.hh
 *
 * @date Jun 3, 2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FASTEVENTACTION_HH_
#define FASTEVENTACTION_HH_

#include <EventAction.hh>

#include <boost/format.hpp>

#include "DetectorConstruction.hh"
#include "G4Sipm.hh"
#include "housing/G4SipmHousing.hh"
#include "hit/G4SipmHit.hh"
#include "ParticleSourceMessenger.hh"
#include "rnd/Direction.hh"
#include "rnd/Flat.hh"
#include "rnd/General.hh"
#include "rnd/ThreeVector.hh"
#include "rnd/FixedValue.hh"
#include "FastEffectiveCellsFiredDigitizer.hh"

class FastEventAction: public EventAction {
private:
	ParticleSourceMessenger* messenger;

public:
	FastEventAction() :
			EventAction() {
		messenger = ParticleSourceMessenger::getInstance();
	}

	virtual void EndOfEventAction(const G4Event* event) {
		const unsigned int nParticles = messenger->getNParticles();
		// Init random number generators.
		rnd::ThreeVector* position = new rnd::ThreeVector(
				new rnd::Flat(-messenger->getA() / 2., messenger->getA() / 2.),
				new rnd::Flat(-messenger->getB() / 2., messenger->getB() / 2.), new rnd::FixedValue(0.0, 0.0));
		rnd::RandomDouble* energy =
				messenger->getEInput().empty() ?
						(rnd::RandomDouble*) new rnd::Flat(messenger->getEMin(), messenger->getEMax()) :
						(rnd::RandomDouble*) new rnd::General(messenger->getEInput());
		// Update messenger values.
		if (!messenger->getEInput().empty()) {
			G4UImanager::GetUIpointer()->ApplyCommand(
					boost::str(
							boost::format("/ps/energy/eMin %g eV")
									% (((rnd::General*) energy)->getXMin() / CLHEP::eV)));
			G4UImanager::GetUIpointer()->ApplyCommand(
					boost::str(
							boost::format("/ps/energy/eMax %g eV")
									% (((rnd::General*) energy)->getXMax() / CLHEP::eV)));
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
		// Get hits collection
		G4Sipm* sipm =
				((DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction())->getSipmHousing()->getSipm();
		G4SipmHitsCollection* hc = (G4SipmHitsCollection*) event->GetHCofThisEvent()->GetHC(0);
		// Fill hits collection.
		for (size_t i = 0; i < nParticles; i++) {
			double eKin = energy->shoot();
			// Dice PDE.
			if (CLHEP::RandFlat::shoot()
					> sipm->getModel()->getPhotonDetectionEfficiency(CLHEP::c_light * CLHEP::h_Planck / eKin)) {
				continue;
			}
			// Create hit.
			G4SipmHit* h = new G4SipmHit();
			h->setSipmId(sipm->getId());
			// Randomize position.
			h->setPosition(position->shoot());
			// Randomize time.
			h->setTime(time->shoot());
			// Randomize energy.
			h->setEKin(eKin);
			h->setWeight(1);
			//
			hc->insert(h);
		}
		// Add digitizer module.
		new FastEffectiveCellsFiredDigitizer(sipm);
		//
		EventAction::EndOfEventAction(event);
	}
};

#endif /* FASTEVENTACTION_HH_ */
