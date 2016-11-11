/*
 * JsonPersistVisitor.cc
 *
 * @date Feb 11, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "persistency/JsonPersistVisitor.hh"

#include <sstream>

#include <boost/algorithm/string.hpp>

#include <jansson.h>

#include "persistency/PersistencyHandlerMessenger.hh"

void JsonPersistVisitor::write(json_t* j) {
	const size_t flags = JSON_COMPACT | JSON_ENSURE_ASCII | JSON_PRESERVE_ORDER;
	out << json_dumps(j, flags) << "\n";
	json_decref(j);
}

json_t* JsonPersistVisitor::json_vector(const CLHEP::Hep3Vector& v) const {
	json_t* a = json_array();
	json_array_append(a, json_real(v.x()));
	json_array_append(a, json_real(v.y()));
	json_array_append(a, json_real(v.z()));
	return a;
}

void JsonPersistVisitor::operator ()(G4SipmHit* hit, G4SipmHitsCollection* hc) {
	json_t* j = json_object();
	json_object_set_new(j, "class", json_string("G4SipmHit"));
	json_object_set_new(j, "hitCollection", json_string(hc->GetName()));
	json_object_set_new(j, "eKin", json_real(hit->getEKin()));
	json_object_set_new(j, "momentum", json_vector(hit->getMomentum()));
	json_object_set_new(j, "pdgId", json_integer(hit->getPdgId()));
	json_object_set_new(j, "trackId", json_integer(hit->getTrackId()));
	json_object_set_new(j, "position", json_vector(hit->getPosition()));
	json_object_set_new(j, "sipmId", json_integer(hit->getSipmId()));
	json_object_set_new(j, "startMomentum", json_vector(hit->getStartMomentum()));
	json_object_set_new(j, "startPosition", json_vector(hit->getStartPosition()));
	json_object_set_new(j, "time", json_real(hit->getTime()));
	json_object_set_new(j, "weight", json_real(hit->getWeight()));
	json_object_set_new(j, "worldPosition", json_vector(hit->getWorldPosition()));
	write(j);
}

void JsonPersistVisitor::operator ()(G4SipmDigi* digi, G4SipmDigiCollection* dc) {
	json_t* j = json_object();
	json_object_set_new(j, "class", json_string("G4SipmDigi"));
	json_object_set_new(j, "digiCollection", json_string(dc->GetName()));
	json_object_set_new(j, "sipmId", json_integer(digi->getSipmId()));
	json_object_set_new(j, "cellId", json_integer(digi->getCellId()));
	json_object_set_new(j, "type", json_integer(digi->getType()));
	json_object_set_new(j, "time", json_real(digi->getTime()));
	json_object_set_new(j, "weight", json_real(digi->getWeight()));
	write(j);
}

void JsonPersistVisitor::operator ()(G4SipmVoltageTraceDigi* digi, G4SipmVoltageTraceDigiCollection* dc) {
	// Cache tMin and tMax of the persistency handler.
	const double tMin = std::max(digi->getTMin(), PersistencyHandlerMessenger::getInstance()->getTMin());
	const double tMax = std::min(digi->getTMax(), PersistencyHandlerMessenger::getInstance()->getTMax());
	// Create Json object.
	json_t* j = json_object();
	json_object_set_new(j, "class", json_string("G4SipmVoltageTraceDigi"));
	json_object_set_new(j, "digiCollection", json_string(dc->GetName()));
	json_object_set_new(j, "sipmId", json_integer(digi->getSipmId()));
	json_object_set_new(j, "timeBinWidth", json_real(digi->getTimeBinWidth()));
	json_object_set_new(j, "tMin", json_real(std::max(digi->getTMin(), tMin)));
	json_object_set_new(j, "tMax", json_real(std::min(digi->getTMax(), tMax)));
	// Copy all values which have to be persisted.
	json_t* v = json_array();
	for (double t = tMin; t < tMax; t += digi->getTimeBinWidth()) {
		json_array_append(v, json_real(digi->atMeasured(digi->index(t))));
	}
	json_object_set_new(j, "voltages", v);
	write(j);
}

void JsonPersistVisitor::open(std::string filename) {
	if (!boost::algorithm::ends_with(filename, ".txt")) {
		filename.append(".txt");
	}
	out.open(filename.c_str());
}

void JsonPersistVisitor::operator ()(G4SipmHitsCollection* hc) {
	if (!PersistencyHandlerMessenger::getInstance()->isPersistHits()) {
		return;
	}
	for (size_t i = 0; i < hc->GetSize(); i++) {
		(*this)((G4SipmHit*) hc->GetHit(i), hc);
	}
}

void JsonPersistVisitor::operator ()(G4SipmDigiCollection* dc) {
	if (!PersistencyHandlerMessenger::getInstance()->isPersistDigis()) {
		return;
	}
	const double tMin = PersistencyHandlerMessenger::getInstance()->getTMin();
	const double tMax = PersistencyHandlerMessenger::getInstance()->getTMax();
	for (size_t i = 0; i < dc->GetSize(); i++) {
		G4SipmDigi* digi = (G4SipmDigi*) (dc->GetDigi(i));
		if (digi->getTime() >= tMin && digi->getTime() < tMax) {
			(*this)(digi, dc);
		}
	}
}

void JsonPersistVisitor::operator ()(G4SipmVoltageTraceDigiCollection* dc) {
	for (size_t i = 0; i < dc->GetSize(); i++) {
		(*this)((G4SipmVoltageTraceDigi*) dc->GetDigi(i), dc);
	}
}

void JsonPersistVisitor::close() {
	out.close();
}

void JsonPersistVisitor::operator ()(G4SipmUiMessenger* m) {
	json_t* j = json_object();
	json_object_set_new(j, "class", json_string("G4SipmUiMessenger"));
	json_object_set_new(j, "verbose", json_integer(m->getVerbose()));
	json_object_set_new(j, "seed", json_integer(m->getSeed()));
	json_object_set_new(j, "filterGeometry", json_boolean(m->isFilterGeometry()));
	json_object_set_new(j, "filterPde", json_boolean(m->isFilterPde()));
	json_object_set_new(j, "filterTiming", json_boolean(m->isFilterTiming()));
	json_object_set_new(j, "noiseThermal", json_boolean(m->isNoiseThermal()));
	json_object_set_new(j, "noisePreThermal", json_integer(m->getNoisePreThermal()));
	json_object_set_new(j, "noisePostThermal", json_integer(m->getNoisePostThermal()));
	json_object_set_new(j, "noiseAfterpulse", json_boolean(m->isNoiseAfterpulse()));
	json_object_set_new(j, "noiseCrosstalk", json_boolean(m->isNoiseCrosstalk()));
	json_object_set_new(j, "noiseCrosstalkNeighbours", json_integer(m->getNoiseCrosstalkNeighbours()));
	json_object_set_new(j, "noiseIfNoSignal", json_boolean(m->isNoiseIfNoSignal()));
	json_object_set_new(j, "digitizeHits", json_boolean(m->isDigitizeHits()));
	json_object_set_new(j, "digitizeTrace", json_boolean(m->isDigitizeTrace()));
	json_object_set_new(j, "simulateShuntresistor", json_boolean(m->isSimulateShuntresistor()));
	json_object_set_new(j, "shuntresistorRecoveryTime", json_real(m->getShuntresistorRecoveryTime()));
	write(j);
}

void JsonPersistVisitor::operator ()(ParticleSourceMessenger* m) {
	json_t* j = json_object();
	json_object_set_new(j, "class", json_string("ParticleSourceMessenger"));
	json_object_set_new(j, "verbose", json_integer(m->getVerbose()));
	json_object_set_new(j, "nParticles", json_integer(m->getNParticles()));
	json_object_set_new(j, "polar", json_real(m->getPolar()));
	json_object_set_new(j, "tMin", json_real(m->getTMin()));
	json_object_set_new(j, "tMin", json_real(m->getTMax()));
	json_object_set_new(j, "tInput", json_string(m->getTInput()));
	json_object_set_new(j, "eMin", json_real(m->getEMin()));
	json_object_set_new(j, "eMax", json_real(m->getEMax()));
	json_object_set_new(j, "eInput", json_string(m->getEInput()));
	json_object_set_new(j, "phiMin", json_real(m->getPhiMin()));
	json_object_set_new(j, "phiMax", json_real(m->getPhiMax()));
	json_object_set_new(j, "thetaMin", json_real(m->getThetaMin()));
	json_object_set_new(j, "thetaMax", json_real(m->getThetaMax()));
	json_object_set_new(j, "surfaceNormal", json_vector(m->getSurfaceNormal()));
	json_object_set_new(j, "a", json_real(m->getA()));
	json_object_set_new(j, "b", json_real(m->getB()));
	json_object_set_new(j, "pos", json_vector(m->getPos()));
	write(j);
}

void JsonPersistVisitor::operator ()(G4SipmModel* m) {
	json_t* j = json_object();
	json_object_set_new(j, "class", json_string("G4SipmModel"));
	json_object_set_new(j, "model", json_string(m->getName().c_str()));
	json_object_set_new(j, "numberOfCells", json_integer(m->getNumberOfCells()));
	json_object_set_new(j, "cellPitch", json_real(m->getCellPitch()));
	json_object_set_new(j, "pitch", json_real(m->getPitch()));
	json_object_set_new(j, "fillFactor", json_real(m->getFillFactor()));
	json_object_set_new(j, "temperature", json_real(m->getTemperature()));
	json_object_set_new(j, "overVoltage", json_real(m->getOverVoltage()));
	json_object_set_new(j, "breakdownVoltage", json_real(m->getBreakdownVoltage()));
	json_object_set_new(j, "thermalNoiseRate", json_real(m->getThermalNoiseRate()));
	json_object_set_new(j, "crossTalkProbability", json_real(m->getCrossTalkProbability()));
	json_object_set_new(j, "apProbLong", json_real(m->getApProbLong()));
	json_object_set_new(j, "apProbShort", json_real(m->getApProbShort()));
	json_object_set_new(j, "apTauLong", json_real(m->getApTauLong()));
	json_object_set_new(j, "apTauShort", json_real(m->getApTauLong()));
	json_object_set_new(j, "recoveryTime", json_real(m->getRecoveryTime()));
	json_object_set_new(j, "deadTime", json_real(m->getDeadTime()));
	write(j);
}

void JsonPersistVisitor::operator ()(G4SipmVoltageTraceModel* m) {
	json_t* j = json_object();
	json_object_set_new(j, "class", json_string("G4SipmVoltageTraceModel"));
	json_object_set_new(j, "amplitude", json_real(m->getAmplitude()));
	json_object_set_new(j, "tauFall", json_real(m->getTauFall()));
	json_object_set_new(j, "tauRise", json_real(m->getTauRise()));
	json_object_set_new(j, "timeBinWidth", json_real(m->getTimeBinWidth()));
	json_object_set_new(j, "v0", json_real(m->getV0()));
	json_object_set_new(j, "whiteNoiseSigma", json_real(m->getWhiteNoiseSigma()));
	write(j);
}
