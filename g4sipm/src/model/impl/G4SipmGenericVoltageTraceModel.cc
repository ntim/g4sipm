/*
 * G4SipmGenericVoltageTraceModel.cc
 *
 * @created Apr 30, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <CLHEP/Units/SystemOfUnits.h>

#include "model/impl/G4SipmGenericVoltageTraceModel.hh"

G4SipmGenericVoltageTraceModel::G4SipmGenericVoltageTraceModel() {
	//
}

double G4SipmGenericVoltageTraceModel::getAmplitude() const {
	return 50e-3 * CLHEP::volt;
}

double G4SipmGenericVoltageTraceModel::getTauRise() const {
	return 3. * CLHEP::nanosecond;
}

double G4SipmGenericVoltageTraceModel::getTauFall() const {
	return 40. * CLHEP::nanosecond;
}

double G4SipmGenericVoltageTraceModel::getV0() const {
	return 25e-3 * CLHEP::volt;
}

double G4SipmGenericVoltageTraceModel::getWhiteNoiseSigma() const {
	return 1e-3 * CLHEP::volt;
}

int G4SipmGenericVoltageTraceModel::getPrecision() const {
	return 12;
}

double G4SipmGenericVoltageTraceModel::getTimeBinWidth() const {
	return 1. * CLHEP::nanosecond;
}
