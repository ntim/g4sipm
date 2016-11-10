/*
 * VoltageTraceDigitizer.cc
 *
 * @created Apr 19, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "digi/G4SipmVoltageTraceDigitizer.hh"

#include <G4DigiManager.hh>
#include <G4Timer.hh>

#include <CLHEP/Random/RandGauss.h>

#include "G4SipmUiMessenger.hh"

G4SipmVoltageTraceDigitizer::G4SipmVoltageTraceDigitizer(G4Sipm* _sipm) :
		G4VDigitizerModule(_sipm->getVoltageTraceDigitizerName()), sipm(_sipm) {
	collectionName.push_back(sipm->getVoltageTraceDigiCollectionName());
	// Register to Geant4.
	G4DigiManager::GetDMpointer()->AddNewModule(this);
}

G4SipmVoltageTraceDigitizer::~G4SipmVoltageTraceDigitizer() {
	//
}

const G4SipmDigiCollection* G4SipmVoltageTraceDigitizer::getDigiCollection() {
	G4DigiManager* digiManager = G4DigiManager::GetDMpointer();
	G4int collId = digiManager->GetDigiCollectionID(sipm->getDigiCollectionName());
	if (collId >= 0) {
		const G4SipmDigiCollection* digiCollection =
				static_cast<const G4SipmDigiCollection*>(digiManager->GetDigiCollection(collId));
		return digiCollection;
	}
	return NULL;
}

void G4SipmVoltageTraceDigitizer::Digitize() {
	if (!sipm || !G4SipmUiMessenger::getInstance()->isDigitizeTrace()) {
		return;
	}
	G4Timer timer;
	timer.Start();
	// Get Digi collection.
	const G4SipmDigiCollection* digis = getDigiCollection();
	if (!digis || digis->GetSize() == 0) {
		return;
	}
	G4SipmVoltageTraceModel* voltageTraceModel = sipm->getModel()->getVoltageTraceModel();
	// Assuming digis are ordered chronologically, get the first and last.
	double tMin = reinterpret_cast<G4SipmDigi*>(digis->GetVector()->front())->getTime();
	double tMax = reinterpret_cast<G4SipmDigi*>(digis->GetVector()->back())->getTime()
			+ 10. * voltageTraceModel->getTauFall();
	// Create trace.
	G4SipmVoltageTraceDigi* trace = new G4SipmVoltageTraceDigi(sipm->getId(), tMin, tMax,
			voltageTraceModel->getTimeBinWidth());
	trace->setPrecision(voltageTraceModel->getPrecision());
	for (size_t i = 0; i < digis->GetSize(); i++) {
		G4SipmDigi* d = reinterpret_cast<G4SipmDigi*>(digis->GetDigi(i));
		// Add trigger to trace.
		double ti = 0.;
		while (ti < 10. * voltageTraceModel->getTauFall()) {
			size_t index = trace->index(ti + d->getTime());
			if (index < trace->size()) {
				double vi = voltageTraceModel->pulse(ti, d->getWeight());
				// If pulse gets really small break early.
				if (ti > voltageTraceModel->getTauRise() && vi < 1e-4 * voltageTraceModel->getAmplitude()) {
					break;
				}
				trace->at(index) += vi;
			}
			ti += voltageTraceModel->getTimeBinWidth();
		}
	}
	// Add baseline and white noise.
	std::vector<double> avg(4, 0);
	for (size_t i = 0; i < trace->size(); i++) {
		// Compute 3 point moving average of white noise.
		avg[i % avg.size()] = CLHEP::RandGauss::shoot(0., voltageTraceModel->getWhiteNoiseSigma());
		trace->at(i) += voltageTraceModel->getV0() + (avg[0] + avg[1] + avg[2]) / static_cast<double>(avg.size());
	}
	// Store trace.
	G4SipmVoltageTraceDigiCollection* traces = new G4SipmVoltageTraceDigiCollection(GetName(), GetCollectionName(0));
	traces->insert(trace);
	StoreDigiCollection(traces);
	// Stop timer and print information.
	timer.Stop();
	std::cout << "G4SipmVoltageTraceDigitizer::Digitize(): added digi collection \"" << traces->GetName()
			<< "\" for digitizer \"" << GetName() << "\" with " << traces->GetSize() << " entries " << "in (" << tMin
			<< ", " << tMax << ") ns " << "(" << timer << ")." << std::endl;
}

G4Sipm* G4SipmVoltageTraceDigitizer::getSipm() const {
	return sipm;
}

void G4SipmVoltageTraceDigitizer::setSipm(G4Sipm* _sipm) {
	sipm = _sipm;
}
