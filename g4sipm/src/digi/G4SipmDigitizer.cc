/*
 * Digitizer.cc
 *
 * @created Apr 11, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "digi/G4SipmDigitizer.hh"

#include <G4DigiManager.hh>
#include <G4Timer.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Random/Random.h>
#include <CLHEP/Random/Ranlux64Engine.h>
#include <CLHEP/Random/RandExponential.h>
#include <CLHEP/Random/RandPoisson.h>
#include <CLHEP/Random/RandFlat.h>
#include <CLHEP/Random/RandBinomial.h>

#include <sys/timeb.h>
#include <math.h>
#include <set>

#include "G4SipmUiMessenger.hh"
#include "digi/G4SipmCellFireController.hh"
#include "digi/G4SipmEffectiveOvervoltageCellFireController.hh"

G4SipmDigitizer::G4SipmDigitizer(G4Sipm* _sipm) :
		G4VDigitizerModule(_sipm->getDigitizerName()), sipm(_sipm) {
	// Add digi collection name.
	collectionName.push_back(sipm->getDigiCollectionName());
	// Register to Geant4.
	G4DigiManager::GetDMpointer()->AddNewModule(this);
}

G4SipmDigitizer::~G4SipmDigitizer() {
	//
}

void G4SipmDigitizer::addCrosstalk(G4SipmDigi* const d, G4SipmDigiQueue* queue) {
	G4SipmModel* model = sipm->getModel();
	// Binomial process with 4 trials (neighbors) for crosstalk.
	const unsigned int nTrials = G4SipmUiMessenger::getInstance()->getNoiseCrosstalkNeighbours();
	int shells = nTrials > 4 ? (sqrt(1 + nTrials) - 1) / 2 : 1;
	double epsilon = 1. - pow(1. - getRecoveryCorrectedProbability(d, model->getCrossTalkProbability()), 1. / nTrials);
	if (epsilon <= 1.e-9) {
		return;
	}
	unsigned int nCt = CLHEP::RandBinomial::shoot(nTrials, epsilon);
	for (unsigned int i = 0; i < nCt; i++) {
		G4SipmDigi* dct = new G4SipmDigi(*d);
		dct->setType(CROSSTALK);
		CLHEP::Hep2Vector pos = model->getCellPosition(d->getCellId());
		// Dice new id.
		G4SipmCellId cellId = d->getCellId();
		double a = 0, b = 0;
		while (a == 0 && b == 0) {
			a = CLHEP::RandFlat::shootInt(-shells, shells + 1);
			b = CLHEP::RandFlat::shootInt(-shells, shells + 1);
			// Only for four nearest neighbors.
			if (nTrials == 4) {
				if (a != 0) {
					b = 0;
				} else if (b != 0) {
					a = 0;
				}
			}
		}
		double x = a * model->getCellPitch() + pos.x();
		double y = b * model->getCellPitch() + pos.y();
		cellId = model->getCellId(x, y, false);
		if (model->isValidCellId(cellId)) {
			dct->setCellId(cellId);
			queue->push(dct);
		}
	}
}

void G4SipmDigitizer::addAfterpulses(G4SipmDigi* const d, G4SipmDigiQueue* queue) {
	G4SipmModel* model = sipm->getModel();
	addAfterpulses(d, queue, model->getApProbLong(), model->getApTauLong());
	addAfterpulses(d, queue, model->getApProbShort(), model->getApTauShort());
}

void G4SipmDigitizer::addAfterpulses(G4SipmDigi* const d, G4SipmDigiQueue* queue, const double probability,
		const double tau) {
	// Poisson distributed number.
	const double mean = -log(1. - getRecoveryCorrectedProbability(d, probability));
	unsigned int nAp = CLHEP::RandPoisson::shoot(mean);
	// Bernoulli process.
	for (unsigned int i = 0; i < nAp; i++) {
		G4SipmDigi* dap = new G4SipmDigi(*d);
		dap->setType(AFTERPULSE);
		// Dice time.
		dap->setTime(d->getTime() + CLHEP::RandExponential::shoot(tau));
		queue->push(dap);
	}
}

double G4SipmDigitizer::getRecoveryCorrectedProbability(G4SipmDigi* const d, double prob) const {
	// The weight equals the recharge status of the cell.
	// Measurements confirm a quadratic dependence of the probabilities to the overvoltage.
	return prob * d->getWeight() * d->getWeight();
}

void G4SipmDigitizer::addHits(G4SipmDigiQueue* queue, double* tMin, double* tMax) {
	const G4SipmHitsCollection* hits = getHitCollection();
	if (!hits || hits->GetSize() == 0) {
		return;
	}
	*tMin = reinterpret_cast<G4SipmHit*>(hits->GetHit(0))->getTime();
	*tMax = *tMin;
	for (size_t i = 0; i < hits->GetSize(); i++) {
		G4SipmHit* hit = reinterpret_cast<G4SipmHit*>(hits->GetHit(i));
		G4SipmDigi* d = new G4SipmDigi;
		d->setType(PHOTON);
		d->setSipmId(sipm->getId());
		d->setCellId(sipm->getModel()->getCellId(hit->getPosition().x(), hit->getPosition().y(), false));
		d->setTime(hit->getTime());
		d->setWeight(hit->getWeight());
		queue->push(d);
		// Remember minimum and maximum time.
		*tMin = std::min(*tMin, hit->getTime());
		*tMax = std::max(*tMax, hit->getTime());
	}
}

void G4SipmDigitizer::addThermalNoise(G4SipmDigiQueue* queue, const double tMin, const double tMax) {
	G4Timer timer;
	timer.Start();
	if (sipm->getModel()->getThermalNoiseRate() == 0) {
		std::cerr << "G4SipmDigitizer::addThermalNoise(): thermal noise rate zero -> skipping." << std::endl;
		return;
	}
	// Number of cells.
	const unsigned int nCells = sipm->getModel()->getNumberOfCells();
	// Thermal noise rate on single cell basis.
	const double tau = 1. / sipm->getModel()->getThermalNoiseRate();
	if (tau > 1. * CLHEP::second) {
		std::cerr
				<< "G4SipmDigitizer::addThermalNoise(): thermal noise time constant large than one second -> skipping."
				<< std::endl;
		return;
	}
	// Dice ...
	double tFirst = -((double) G4SipmUiMessenger::getInstance()->getNoisePreThermal() * (double) nCells)
			/ sipm->getModel()->getThermalNoiseRate() - tMin;
	double tLast = ((double) G4SipmUiMessenger::getInstance()->getNoisePostThermal() * (double) nCells)
			/ sipm->getModel()->getThermalNoiseRate() + tMax;
	for (double t = tFirst; t < tLast; t += CLHEP::RandExponential::shoot(tau)) {
		G4SipmDigi* d = new G4SipmDigi();
		d->setSipmId(sipm->getId());
		d->setCellId(CLHEP::RandFlat::shootInt(nCells));
		d->setType(THERMAL);
		d->setTime(t);
		queue->push(d);
	}
	if (verboseLevel > 0) {
		timer.Stop();
		std::cout << "G4SipmDigitizer::addThermalNoise(): \"" << GetName() << "\": generated " << queue->size()
				<< " thermal noise digis in (" << tFirst / CLHEP::microsecond << " us, " << tLast / CLHEP::microsecond
				<< " us) (" << timer << ")." << std::endl;
	}
}

void G4SipmDigitizer::Digitize() {
	if (!sipm) {
		return;
	}
	// Start a timer.
	G4Timer timer;
	timer.Start();
	// Set verbosity.
	SetVerboseLevel(G4SipmUiMessenger::getInstance()->getVerbose());
	// Check if digitization is enabled.
	if (!G4SipmUiMessenger::getInstance()->isDigitizeHits()) {
		return;
	}
	// Add signal.
	G4SipmDigiQueue queue;
	double tMin = 0, tMax = 0;
	addHits(&queue, &tMin, &tMax);
	// If queue is empty...
	if (queue.empty() && !G4SipmUiMessenger::getInstance()->isNoiseIfNoSignal()) {
		std::cout << "G4SipmDigitizer::Digitize(): \"" << GetName() << "\": hit collection empty." << std::endl;
		return;
	}
	std::cout << "G4SipmDigitizer::Digitize(): \"" << GetName() << "\": created " << queue.size() << " photon hits."
			<< std::endl;
	if (G4SipmUiMessenger::getInstance()->isNoiseThermal()) {
		// Add thermal noise.
		addThermalNoise(&queue, tMin, tMax);
	}
	// Iterate queue.
	double t0 = queue.empty() ? 0 : queue.top()->getTime();
	G4SipmCellFireController* controller =
			G4SipmUiMessenger::getInstance()->isSimulateShuntresistor() ?
					new G4SipmEffectiveOvervoltageCellFireController(sipm->getModel(), t0) :
					new G4SipmCellFireController(sipm->getModel(), t0);
	G4SipmDigiCollection* fired = new G4SipmDigiCollection(GetName(), GetCollectionName(0));
	while (queue.hasNext()) {
		G4SipmDigi* d = queue.next();
		if (controller->fire(d)) {
			fired->insert(d);
			// Handle crosstalk.
			if (G4SipmUiMessenger::getInstance()->isNoiseCrosstalk()) {
				addCrosstalk(d, &queue);
			}
			// Handle afterpulsing.
			if (G4SipmUiMessenger::getInstance()->isNoiseAfterpulse()) {
				addAfterpulses(d, &queue);
			}
		}
	}
	// Store digis.
	StoreDigiCollection(fired);
	// Free space.
	delete controller;
	// Stop timer and print information.
	timer.Stop();
	std::cout << "G4SipmDigitizer::Digitize(): added digi collection \"" << fired->GetName() << "\" for digitizer \""
			<< GetName() << "\" with " << fired->GetSize() << " entries (" << timer << ")." << std::endl;
}

const G4SipmHitsCollection* G4SipmDigitizer::getHitCollection() {
	// We search and retrieve the hits collection.
	G4DigiManager* digiManager = G4DigiManager::GetDMpointer();
	G4int collId = digiManager->GetHitsCollectionID(sipm->getHitCollectionName());
	if (collId >= 0) {
		const G4SipmHitsCollection* hitsCollection =
				static_cast<const G4SipmHitsCollection*>(digiManager->GetHitsCollection(collId));
		return hitsCollection;
	}
	return NULL;
}

G4Sipm* G4SipmDigitizer::getSipm() const {
	return sipm;
}

void G4SipmDigitizer::setSipm(G4Sipm* _sipm) {
	sipm = _sipm;
}
