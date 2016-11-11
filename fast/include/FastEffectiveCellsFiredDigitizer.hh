/*
 * FastEffectiveCellsFiredDigitizer.hh
 *
 * @date Jun 5, 2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FASTEFFECTIVECELLSFIREDDIGITIZER_HH_
#define FASTEFFECTIVECELLSFIREDDIGITIZER_HH_

#include <G4VDigitizerModule.hh>

#include <G4DigiManager.hh>
#include <G4Timer.hh>

#include "digi/G4SipmDigi.hh"
#include "ParticleSourceMessenger.hh"
#include "G4SipmUiMessenger.hh"

class FastEffectiveCellsFiredDigitizer: public G4VDigitizerModule {
private:
	G4Sipm* sipm;

	const G4SipmDigiCollection* getDigiCollection() {
		G4DigiManager* digiManager = G4DigiManager::GetDMpointer();
		G4int collId = digiManager->GetDigiCollectionID(sipm->getDigiCollectionName());
		if (collId >= 0) {
			const G4SipmDigiCollection* digiCollection =
					static_cast<const G4SipmDigiCollection*>(digiManager->GetDigiCollection(collId));
			return digiCollection;
		}
		return NULL;
	}

public:
	FastEffectiveCellsFiredDigitizer(G4Sipm* _sipm) :
			G4VDigitizerModule(_sipm->getDigitizerName() + "-fast"), sipm(_sipm) {
		// Add digi collection name.
		collectionName.push_back(sipm->getDigiCollectionName() + "-fast");
		// Register to Geant4.
		G4DigiManager::GetDMpointer()->AddNewModule(this);
	}

	virtual void Digitize() {
		if (!sipm) {
			return;
		}
		// Start a timer.
		G4Timer timer;
		timer.Start();
		// Get Digi collection.
		const G4SipmDigiCollection* digis = getDigiCollection();
		if (!digis || digis->GetSize() == 0) {
			return;
		}
		double integral = 0.0;
		const double tMin = ParticleSourceMessenger::getInstance()->getTMin();
		const double tMax = ParticleSourceMessenger::getInstance()->getTMax();
		for (size_t i = 0; i < digis->GetSize(); i++) {
			G4SipmDigi* digi = (G4SipmDigi*) (digis->GetDigi(i));
			if (digi->getTime() >= tMin && digi->getTime() < tMax) {
				integral += digi->getWeight();
			}
		}
		// Replace the complete content of the digi collection with one file containing the integral.
		G4SipmDigi* digi = new G4SipmDigi();
		digi->setSipmId(sipm->getId());
		digi->setTime(tMin + (tMax - tMin) / 2.0);
		digi->setType(UNDEFINED);
		digi->setCellId(0);
		digi->setWeight(integral);
		// Create new digi collection
		G4SipmDigiCollection* repl = new G4SipmDigiCollection(sipm->getDigitizerName(), sipm->getDigiCollectionName());
		repl->insert(digi);
		// With the id of the current digi collection, replace the contents.
		StoreDigiCollection(G4DigiManager::GetDMpointer()->GetDigiCollectionID(sipm->getDigiCollectionName()), repl);
		//
		timer.Stop();
		std::cout << "FastEffectiveCellsFiredDigitizer::Digitize(): replaced digi collection \"" << repl->GetName()
				<< "\" for digitizer \"" << GetName() << "\" with integral " << integral << " p.e. from " << tMin
				<< " ns to " << tMax << " ns " << "(" << timer << ")." << std::endl;
	}

};

#endif /* FASTEFFECTIVECELLSFIREDDIGITIZER_HH_ */
