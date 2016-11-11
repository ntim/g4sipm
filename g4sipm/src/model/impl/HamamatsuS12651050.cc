/*
 * HamamatsuS12651050.cc
 *
 * @date Oct 16, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/impl/HamamatsuS12651050.hh"

#include <VectorUtil.hh>
#include <InterpolationUtil.hh>
#include <model/impl/G4SipmRandomGainMapModel.hh>
#include <model/impl/G4SipmGenericVoltageTraceModel.hh>

#include <CLHEP/Units/SystemOfUnits.h>

HamamatsuS12651050::HamamatsuS12651050() :
		G4SipmModel(new G4SipmRandomGainMapModel, new G4SipmGenericVoltageTraceModel) {
	setBiasVoltage(getBreakdownVoltage() + 2.6 * CLHEP::volt);
	// Absolute PDE (First point only an estimation).
	// Data from Tadday (2010).
	static const double x[] = { 280, 350, 360, 370, 380, 390, 400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500,
			510, 520, 530, 540, 550, 560, 570, 580, 590, 600, 610, 620, 630, 640, 650, 660, 670, 680, 690, 700, 710,
			720, 730, 740, 750, 760, 770, 780, 790, 800, 810, 820, 830, 840, 850, 860, 870, 880, 890, 900, 910, 920,
			930, 940, 950, 960, 970, 980, 990, 1000, };
	static const double y[] = { 0, 35, 33.25, 33, 33.5, 34.5, 34.95, 35.5, 35.6, 36.9, 36.9, 36.7, 35.95, 34.3, 35.1,
			34.5, 33.95, 33.05, 31.9, 30.75, 29.5, 28.4, 26.9, 25.6, 24.65, 23.55, 22.55, 21.55, 20.55, 19.5, 18.66,
			17.75, 16.9, 16.2, 15.33, 14.5, 13.8, 13.05, 12.3, 11.5, 10.9, 10.2, 9.55, 8.75, 8.15, 7.66, 7.1, 6.5, 6.05,
			5.66, 5.1, 4.6, 4.2, 3.85, 3.6, 3.2, 2.85, 2.5, 2.33, 2, 1.8, 1.6, 1.3, 1.05, 1, 0.75, 0.55, };
	wvl = std::vector<double>(x, x + sizeof(x) / sizeof(x[0]));
	pde = std::vector<double>(y, y + sizeof(y) / sizeof(y[0]));
	// Apply units.
	wvl = VectorUtil::times(wvl, CLHEP::nanometer);
	pde = VectorUtil::times(pde, CLHEP::perCent);
	//
	getGainMapModel()->refresh(this);
}

std::string HamamatsuS12651050::getName() const {
	return "Hamamatsu-S12651-050";
}

double HamamatsuS12651050::getBreakdownVoltage() const {
	return 65. * CLHEP::volt;
}

unsigned int HamamatsuS12651050::getNumberOfCells() const {
	return 400;
}

double HamamatsuS12651050::getCellPitch() const {
	return 50. * CLHEP::micrometer;
}

double HamamatsuS12651050::getThermalNoiseRate() const {
	return 100. * CLHEP::kilohertz;
}

double HamamatsuS12651050::getDeadTime() const {
	return 1. * CLHEP::picosecond;
}

double HamamatsuS12651050::getRecoveryTime() const {
	return 30. * CLHEP::nanosecond;
}

double HamamatsuS12651050::getCrossTalkProbability() const {
	return 5. * CLHEP::perCent;
}

double HamamatsuS12651050::getApProbLong() const {
	return 1. - sqrt(1. - 3. * CLHEP::perCent);
}

double HamamatsuS12651050::getApProbShort() const {
	return 1. - sqrt(1. - 3. * CLHEP::perCent);
}

double HamamatsuS12651050::getApTauLong() const {
	return 120. * CLHEP::nanosecond;
}

double HamamatsuS12651050::getApTauShort() const {
	return 40. * CLHEP::nanosecond;
}

double HamamatsuS12651050::getFillFactor() const {
	return 80. * CLHEP::perCent;
}

double HamamatsuS12651050::getGainVariation() const {
	return 8.0 * CLHEP::perCent;
}

double HamamatsuS12651050::getPhotonDetectionEfficiency(double wavelength) const {
	return InterpolationUtil::linear(wvl, pde, wavelength);
}
