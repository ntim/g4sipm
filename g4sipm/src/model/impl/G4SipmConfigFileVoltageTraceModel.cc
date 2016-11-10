/*
 * G4SipmConfigFileVoltageTraceModel.cc
 *
 * @created Apr 29, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/impl/G4SipmConfigFileVoltageTraceModel.hh"

#include "Properties.hh"

G4SipmConfigFileVoltageTraceModel::G4SipmConfigFileVoltageTraceModel(std::string filename) :
		G4SipmVoltageTraceModel() {
	Properties p;
	if (!p.load(filename)) {
		throw 1;
	}
	// Set properties.
	amplitude = p.getNumber("voltageTrace-amplitude");
	tauRise = p.getNumber("voltageTrace-tauRise");
	tauFall = p.getNumber("voltageTrace-tauFall");
	v0 = p.getNumber("voltageTrace-v0");
	whiteNoiseSigma = p.getNumber("voltageTrace-whiteNoiseSigma");
	timeBinWidth = p.getNumber("voltageTrace-timeBinWidth");
	precision = p.getNumber("voltageTrace-precision");
}

double G4SipmConfigFileVoltageTraceModel::getAmplitude() const {
	return amplitude;
}

double G4SipmConfigFileVoltageTraceModel::getTauRise() const {
	return tauRise;
}

double G4SipmConfigFileVoltageTraceModel::getTauFall() const {
	return tauFall;
}

double G4SipmConfigFileVoltageTraceModel::getV0() const {
	return v0;
}

double G4SipmConfigFileVoltageTraceModel::getWhiteNoiseSigma() const {
	return whiteNoiseSigma;
}

int G4SipmConfigFileVoltageTraceModel::getPrecision() const {
	return precision;
}

double G4SipmConfigFileVoltageTraceModel::getTimeBinWidth() const {
	return timeBinWidth;
}
