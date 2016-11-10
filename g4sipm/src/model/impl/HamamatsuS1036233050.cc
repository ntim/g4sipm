/*
 * HamamatsuS1036233050.cc
 *
 * @created Feb 13, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/impl/HamamatsuS1036233050.hh"

#include <CLHEP/Units/PhysicalConstants.h>
#include <CLHEP/Units/SystemOfUnits.h>

HamamatsuS1036233050::HamamatsuS1036233050() :
		HamamatsuS1036233100() {
	setBiasVoltage(getBreakdownVoltage() + 1.5 * CLHEP::volt);
	setTemperature(3.8 * CLHEP::kelvin + CLHEP::STP_Temperature);
}

std::string HamamatsuS1036233050::getName() const {
	return "HamamatsuS1036233050";
}

unsigned int HamamatsuS1036233050::getNumberOfCells() const {
	return 3600;
}

double HamamatsuS1036233050::getThermalNoiseRate() const {
	// Measurement Tim Tim Niggemann, III Phys. Inst. A, RWTH Aachen University et. al. 2014.
	return 1.397 * CLHEP::megahertz;
}

double HamamatsuS1036233050::getBreakdownVoltage() const {
	// Measurement Tim Tim Niggemann, III Phys. Inst. A, RWTH Aachen University et. al. 2014.
	return 68.6171 * CLHEP::volt;
}

double HamamatsuS1036233050::getCellPitch() const {
	return 50. * CLHEP::micrometer;
}

double HamamatsuS1036233050::getRecoveryTime() const {
	// Measurement Florian Scheuch master's thesis 2013.
	return 12.59 * CLHEP::nanosecond;
	// Measurement of Lauscher master's thesis 2012.
	// return 19.5 * CLHEP::nanosecond;
}

double HamamatsuS1036233050::getCrossTalkProbability() const {
	// Measurement Tim Tim Niggemann, III Phys. Inst. A, RWTH Aachen University et. al. 2014.
	// return 12.0084 * CLHEP::perCent;
	// Determined from data Navas, Lauscher et al.
	return 15.1 * CLHEP::perCent;
}

double HamamatsuS1036233050::getApProbLong() const {
	// Measurement Tim Tim Niggemann, III Phys. Inst. A, RWTH Aachen University et. al. 2014.
	return 0.0915565;
}

double HamamatsuS1036233050::getApProbShort() const {
	// Measurement Tim Tim Niggemann, III Phys. Inst. A, RWTH Aachen University et. al. 2014.
	return 0.132401;
}

double HamamatsuS1036233050::getApTauLong() const {
	// Measurement Tim Tim Niggemann, III Phys. Inst. A, RWTH Aachen University et. al. 2014.
	return 118.918 * CLHEP::nanosecond;
}

double HamamatsuS1036233050::getApTauShort() const {
	// Measurement Tim Tim Niggemann, III Phys. Inst. A, RWTH Aachen University et. al. 2014.
	return 27.432 * CLHEP::nanosecond;
}

double HamamatsuS1036233050::getFillFactor() const {
	return 61.5 * CLHEP::perCent;
}

double HamamatsuS1036233050::getDeadTime() const {
	return HamamatsuS1036233100::getDeadTime();
}

double HamamatsuS1036233050::getPhotonDetectionEfficiency(double) const {
	// Measurement Tadday et. al. 2010. folded with LED spectrum.
	// return 23.4 * CLHEP::perCent;
	// Determined from data Navas, Lauscher et al.
	return 18.4 * CLHEP::perCent;
	// return 23.4 * CLHEP::perCent;
}
