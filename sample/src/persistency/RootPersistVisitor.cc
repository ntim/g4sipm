/*
 * RootPersistVisitor.cc
 *
 * @date Feb 12, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "persistency/RootPersistVisitor.hh"

#include <boost/algorithm/string.hpp>

#include <TFile.h>
#include <TTree.h>
#include <TString.h>

#include "persistency/PersistencyHandlerMessenger.hh"

void RootPersistVisitor::open(std::string filename) {
	if (!boost::algorithm::ends_with(filename, ".root")) {
		filename.append(".root");
	}
	file = new TFile(filename.c_str(), "RECREATE");
}

void RootPersistVisitor::copy(CLHEP::Hep3Vector v, double* a) {
	a[0] = v.x();
	a[1] = v.y();
	a[2] = v.z();
}

void RootPersistVisitor::operator ()(G4SipmHitsCollection* hc) {
	if (!PersistencyHandlerMessenger::getInstance()->isPersistHits()) {
		return;
	}
	TTree* t = new TTree(hc->GetName(), hc->GetName());
	int sipmId, pdgId, trackId;
	double eKin, time, weight;
	double *momentum = new double[3], *position = new double[3], *worldPosition = new double[3], *startMomentum =
			new double[3], *startPosition = new double[3];
	t->Branch("sipmId", &sipmId);
	t->Branch("pdgId", &pdgId);
	t->Branch("trackId", &trackId);
	t->Branch("eKin", &eKin);
	t->Branch("time", &time);
	t->Branch("weight", &weight);
	t->Branch("momentum[3]", momentum);
	t->Branch("position[3]", position);
	t->Branch("worldPosition[3]", worldPosition);
	t->Branch("startMomentum[3]", startMomentum);
	t->Branch("startPosition[3]", startPosition);
	for (size_t i = 0; i < hc->GetSize(); i++) {
		G4SipmHit* hit = (G4SipmHit*) hc->GetHit(i);
		sipmId = hit->getSipmId();
		pdgId = hit->getPdgId();
		trackId = hit->getTrackId();
		eKin = hit->getEKin();
		time = hit->getTime();
		weight = hit->getWeight();
		copy(hit->getMomentum(), momentum);
		copy(hit->getStartPosition(), position);
		copy(hit->getWorldPosition(), worldPosition);
		copy(hit->getStartMomentum(), startMomentum);
		copy(hit->getStartPosition(), startPosition);
		t->Fill();
	}
	file->WriteTObject(t);
}

void RootPersistVisitor::operator ()(G4SipmDigiCollection* dc) {
	if (!PersistencyHandlerMessenger::getInstance()->isPersistDigis()) {
		return;
	}
	TTree* t = new TTree(dc->GetName(), dc->GetName());
	int sipmId, cellId, type;
	double time, weight;
	t->Branch("sipmId", &sipmId);
	t->Branch("cellId", &cellId);
	t->Branch("type", &type);
	t->Branch("time", &time);
	t->Branch("weight", &weight);
	const double tMin = PersistencyHandlerMessenger::getInstance()->getTMin();
	const double tMax = PersistencyHandlerMessenger::getInstance()->getTMax();
	for (size_t i = 0; i < dc->GetSize(); i++) {
		G4SipmDigi* digi = (G4SipmDigi*) dc->GetDigi(i);
		if (digi->getTime() >= tMin && digi->getTime() < tMax) {
			sipmId = digi->getSipmId();
			cellId = digi->getCellId();
			type = digi->getType();
			time = digi->getTime();
			weight = digi->getWeight();
			t->Fill();
		}
	}
	file->WriteTObject(t);
}

void RootPersistVisitor::operator ()(G4SipmVoltageTraceDigiCollection* dc) {
	TTree* t = new TTree(dc->GetName(), dc->GetName());
	int sipmId;
	double timeBinWidht, time, voltage;
	t->Branch("sipmId", &sipmId);
	t->Branch("timeBinWidht", &timeBinWidht);
	t->Branch("time", &time);
	t->Branch("voltage", &voltage);
	for (size_t i = 0; i < dc->GetSize(); i++) {
		G4SipmVoltageTraceDigi* digi = (G4SipmVoltageTraceDigi*) dc->GetDigi(i);
		sipmId = digi->getSipmId();
		timeBinWidht = digi->getTimeBinWidth();
		const double tMin = std::max(digi->getTMin(), PersistencyHandlerMessenger::getInstance()->getTMin());
		const size_t jMin = digi->index(tMin);
		const double tMax = std::min(digi->getTMax(), PersistencyHandlerMessenger::getInstance()->getTMax());
		const size_t jMax = std::min(digi->size(), digi->index(tMax));
		for (size_t j = jMin; j < jMax; j++) {
			time = digi->time(j);
			voltage = digi->atMeasured(j);
			t->Fill();
		}
	}
	file->WriteTObject(t);
}

void RootPersistVisitor::close() {
	file->Close();
}

void RootPersistVisitor::operator ()(G4SipmUiMessenger* m) {
	TTree* t = new TTree("g4sipmUiMessenger", "g4sipmUiMessenger");
	int verbose = m->getVerbose();
	long seed = m->getSeed();
	bool filterGeometry = m->isFilterGeometry();
	bool filterPde = m->isFilterPde();
	bool filterTiming = m->isFilterTiming();
	bool noiseThermal = m->isNoiseThermal();
	unsigned int noisePreThermal = m->getNoisePreThermal();
	unsigned int noisePostThermal = m->getNoisePostThermal();
	bool noiseAfterpulse = m->isNoiseAfterpulse();
	bool noiseCrosstalk = m->isNoiseCrosstalk();
	unsigned int noiseCrosstalkNeighbours = m->getNoiseCrosstalkNeighbours();
	bool noiseIfNoSignal = m->isNoiseIfNoSignal();
	bool digitizeHits = m->isDigitizeHits();
	bool digitizeTrace = m->isDigitizeTrace();
	bool simulateShuntresistor = m->isSimulateShuntresistor();
	double shuntresistorRecoveryTime = m->getShuntresistorRecoveryTime();
	t->Branch("verbose", &verbose);
	t->Branch("seed", &seed, "seed/L");
	t->Branch("filterGeometry", &filterGeometry);
	t->Branch("filterPde", &filterPde);
	t->Branch("filterTiming", &filterTiming);
	t->Branch("noiseThermal", &noiseThermal);
	t->Branch("noisePreThermal", &noisePreThermal);
	t->Branch("noisePostThermal", &noisePostThermal);
	t->Branch("noiseAfterpulse", &noiseAfterpulse);
	t->Branch("noiseCrosstalk", &noiseCrosstalk);
	t->Branch("noiseCrosstalkNeighbours", &noiseCrosstalkNeighbours);
	t->Branch("noiseIfNoSignal", &noiseIfNoSignal);
	t->Branch("digitizeHits", &digitizeHits);
	t->Branch("digitizeTrace", &digitizeTrace);
	t->Branch("simulateShuntresistor", &simulateShuntresistor);
	t->Branch("shuntresistorRecoveryTime", &shuntresistorRecoveryTime);
	t->Fill();
	file->WriteTObject(t);
}

void RootPersistVisitor::operator ()(ParticleSourceMessenger* m) {
	TTree* t = new TTree("particleSourceMessenger", "particleSourceMessenger");
	int verbose = m->getVerbose();
	int nParticles = m->getNParticles();
	double polar = m->getPolar();
	double tMin = m->getTMin();
	double tMax = m->getTMax();
	TString* tInput = new TString(m->getTInput());
	double eMin = m->getEMin();
	double eMax = m->getEMax();
	TString* eInput = new TString(m->getEInput());
	double phiMin = m->getPhiMin();
	double phiMax = m->getPhiMax();
	double thetaMin = m->getThetaMin();
	double thetaMax = m->getThetaMax();
	double* surfaceNormal = new double[3];
	copy(m->getSurfaceNormal(), surfaceNormal);
	double a = m->getA();
	double b = m->getB();
	double* pos = new double[3];
	copy(m->getPos(), pos);
	t->Branch("verbose", &verbose);
	t->Branch("nParticles", &nParticles);
	t->Branch("polar", &polar);
	t->Branch("tMin", &tMin);
	t->Branch("tMax", &tMax);
	t->Branch("tInput", &tInput);
	t->Branch("eMin", &eMin);
	t->Branch("eMax", &eMax);
	t->Branch("eInput", &eInput);
	t->Branch("phiMin", &phiMin);
	t->Branch("phiMax", &phiMax);
	t->Branch("thetaMin", &thetaMin);
	t->Branch("thetaMax", &thetaMax);
	t->Branch("surfaceNormal[3]", surfaceNormal);
	t->Branch("a", a);
	t->Branch("b", b);
	t->Branch("pos[3]", pos);
	t->Fill();
	file->WriteTObject(t);
}

void RootPersistVisitor::operator ()(G4SipmModel* m) {
	TTree* t = new TTree("sipmModel", "sipmModel");
	TString* name = new TString(m->getName());
	unsigned int numberOfCells = m->getNumberOfCells();
	double cellPitch = m->getCellPitch();
	double pitch = m->getPitch();
	double fillFactor = m->getFillFactor();
	double temperature = m->getTemperature();
	double overVoltage = m->getOverVoltage();
	double breakdownVoltage = m->getBreakdownVoltage();
	double thermalNoiseRate = m->getThermalNoiseRate();
	double crossTalkProbability = m->getCrossTalkProbability();
	double apProbLong = m->getApProbLong();
	double apProbShort = m->getApProbShort();
	double apTauLong = m->getApTauLong();
	double apTauShort = m->getApTauShort();
	double recoveryTime = m->getRecoveryTime();
	double deadTime = m->getDeadTime();
	t->Branch("model", &name);
	t->Branch("numberOfCells", &numberOfCells);
	t->Branch("cellPitch", &cellPitch);
	t->Branch("pitch", &pitch);
	t->Branch("fillFactor", &fillFactor);
	t->Branch("temperature", &temperature);
	t->Branch("overVoltage", &overVoltage);
	t->Branch("breakdownVoltage", &breakdownVoltage);
	t->Branch("thermalNoiseRate", &thermalNoiseRate);
	t->Branch("crossTalkProbability", &crossTalkProbability);
	t->Branch("apProbLong", &apProbLong);
	t->Branch("apProbShort", &apProbShort);
	t->Branch("apTauLong", &apTauLong);
	t->Branch("apTauShort", &apTauShort);
	t->Branch("recoveryTime", &recoveryTime);
	t->Branch("deadTime", &deadTime);
	t->Fill();
	file->WriteTObject(t);
}

void RootPersistVisitor::operator ()(G4SipmVoltageTraceModel* m) {
	TTree* t = new TTree("voltageTraceModel", "voltageTraceModel");
	double amplitude = m->getAmplitude();
	double tauFall = m->getTauFall();
	double tauRise = m->getTauRise();
	double timeBinWidth = m->getTimeBinWidth();
	double v0 = m->getV0();
	double whiteNoiseSigma = m->getWhiteNoiseSigma();
	t->Branch("amplitude", &amplitude);
	t->Branch("tauFall", &tauFall);
	t->Branch("tauRise", &tauRise);
	t->Branch("timeBinWidth", &timeBinWidth);
	t->Branch("v0", &v0);
	t->Branch("whiteNoiseSigma", &whiteNoiseSigma);
	t->Fill();
	file->WriteTObject(t);
}

TFile* RootPersistVisitor::getFile() const {
	return file;
}
