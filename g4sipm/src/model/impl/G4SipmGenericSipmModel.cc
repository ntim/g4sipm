/*
 * GenericSipmModel.cc
 *
 * @created Apr 23, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/impl/G4SipmGenericSipmModel.hh"

#include "model/impl/G4SipmGenericVoltageTraceModel.hh"
#include "model/impl/G4SipmRandomGainMapModel.hh"

#include "CLHEP/Units/SystemOfUnits.h"

G4SipmGenericSipmModel::G4SipmGenericSipmModel() :
		G4SipmModel(new G4SipmRandomGainMapModel, new G4SipmGenericVoltageTraceModel) {
	//
}

std::string G4SipmGenericSipmModel::getName() const {
	return "generic";
}

double G4SipmGenericSipmModel::getBreakdownVoltage() const {
	return 70. * CLHEP::volt;
}

unsigned int G4SipmGenericSipmModel::getNumberOfCells() const {
	return 100;
}

double G4SipmGenericSipmModel::getCellPitch() const {
	return 100 * CLHEP::micrometer;
}

double G4SipmGenericSipmModel::getThermalNoiseRate() const {
	return 1. * CLHEP::megahertz;
}

double G4SipmGenericSipmModel::getDeadTime() const {
	return 1. * CLHEP::picosecond;
}

double G4SipmGenericSipmModel::getRecoveryTime() const {
	return 30. * CLHEP::nanosecond;
}

double G4SipmGenericSipmModel::getCrossTalkProbability() const {
	return 20. * CLHEP::perCent;
}

double G4SipmGenericSipmModel::getApProbLong() const {
	return 10. * CLHEP::perCent;
}

double G4SipmGenericSipmModel::getApProbShort() const {
	return 20. * CLHEP::perCent;
}

double G4SipmGenericSipmModel::getApTauLong() const {
	return 120. * CLHEP::nanosecond;
}

double G4SipmGenericSipmModel::getApTauShort() const {
	return 40. * CLHEP::nanosecond;
}

double G4SipmGenericSipmModel::getFillFactor() const {
	return 78.5 * CLHEP::perCent;
}

double G4SipmGenericSipmModel::getGainVariation() const {
	return 0.5 * CLHEP::perCent;
}

double G4SipmGenericSipmModel::getPhotonDetectionEfficiency(double) const {
	return 30. * CLHEP::perCent;
}
