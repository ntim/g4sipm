/*
 * HamamatsuS1036211100.cc
 *
 * @created Apr 24, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/impl/HamamatsuS1036211100.hh"

#include <CLHEP/Units/SystemOfUnits.h>

#include "VectorUtil.hh"
#include "InterpolationUtil.hh"
#include "model/impl/G4SipmRandomGainMapModel.hh"
#include "model/impl/G4SipmGenericVoltageTraceModel.hh"

HamamatsuS1036211100::HamamatsuS1036211100() :
		G4SipmModel(new G4SipmRandomGainMapModel, new G4SipmGenericVoltageTraceModel) {
	// Properties are given for 1.3 V over-voltage.
	setBiasVoltage(getBreakdownVoltage() + 1.3 * CLHEP::volt);
	// Absolute PDE (First point only an estimation).
	// Data from Tadday (2010).
//	static const double x[] = { 280, 350, 360, 370, 380, 390, 400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500,
//			510, 520, 530, 540, 550, 560, 570, 580, 590, 600, 610, 620, 630, 640, 650, 660, 670, 680, 690, 700, 710,
//			720, 730, 740, 750, 760, 770, 780, 790, 800, 810, 820, 830, 840, 850, 860, 870, 880, 890, 900, 910, 920,
//			930, 940, 950, 960, 970, 980, 990, 1000, };
//	static const double y[] = { 0, 35, 33.25, 33, 33.5, 34.5, 34.95, 35.5, 35.6, 36.9, 36.9, 36.7, 35.95, 34.3, 35.1,
//			34.5, 33.95, 33.05, 31.9, 30.75, 29.5, 28.4, 26.9, 25.6, 24.65, 23.55, 22.55, 21.55, 20.55, 19.5, 18.66,
//			17.75, 16.9, 16.2, 15.33, 14.5, 13.8, 13.05, 12.3, 11.5, 10.9, 10.2, 9.55, 8.75, 8.15, 7.66, 7.1, 6.5, 6.05,
//			5.66, 5.1, 4.6, 4.2, 3.85, 3.6, 3.2, 2.85, 2.5, 2.33, 2, 1.8, 1.6, 1.3, 1.05, 1, 0.75, 0.55, };
	// Data from Hamamatsu scaled by a factor of 0.66 to match measurement by M. Lauscher.
	static const double x[] = { 319.49, 320.882, 322.274, 323.666, 326.45, 327.842, 329.234, 330.626, 332.019, 332.019,
			333.411, 334.803, 336.195, 337.587, 338.979, 340.371, 343.155, 344.548, 345.94, 348.724, 351.508, 355.684,
			358.469, 364.037, 368.213, 372.39, 375.174, 377.958, 382.135, 384.919, 387.703, 390.487, 393.271, 397.448,
			400.232, 404.408, 408.585, 412.761, 418.329, 422.506, 426.682, 432.251, 436.427, 441.995, 447.564, 453.132,
			458.701, 464.269, 468.445, 474.014, 479.582, 483.759, 487.935, 493.503, 497.68, 501.856, 506.032, 510.209,
			514.385, 518.561, 522.738, 526.914, 529.698, 533.875, 538.051, 542.227, 546.404, 550.58, 553.364, 557.541,
			561.717, 565.893, 568.677, 572.854, 577.03, 581.206, 585.383, 588.167, 592.343, 596.52, 600.696, 604.872,
			609.049, 611.833, 616.009, 620.186, 624.362, 628.538, 632.715, 636.891, 641.067, 645.244, 649.42, 653.596,
			657.773, 661.949, 667.517, 671.694, 675.87, 680.046, 684.223, 689.791, 693.968, 699.536, 703.712, 707.889,
			713.457, 717.633, 723.202, 727.378, 732.947, 737.123, 742.691, 746.868, 752.436, 756.613, 762.181, 767.749,
			771.926, 777.494, 781.671, 787.239, 792.807, 796.984, 802.552, 808.121, 812.297, 817.865, 823.434, 827.61,
			833.179, 838.747, 842.923, 848.492, 854.06, 859.629, 865.197, 869.374, 874.942, 880.51, 886.079, 890.255,
			895.824 };
	static const double y[] = { 9.18188, 9.79401, 10.25308, 12.70162, 14.38495, 14.99709, 16.22136, 16.83349, 18.05769,
			18.05769, 19.28196, 19.89409, 20.9653, 21.57743, 22.18956, 22.8017, 23.41383, 24.02597, 24.48503, 25.09717,
			25.55624, 26.01537, 26.47444, 26.93357, 27.23957, 27.69871, 28.15777, 28.61691, 29.07597, 29.68811,
			30.14718, 30.60631, 31.21844, 31.67751, 32.13658, 32.44265, 32.90178, 33.20785, 33.66691, 33.97298,
			34.27905, 34.58512, 34.89118, 35.04418, 35.04418, 35.04418, 34.89118, 34.89118, 34.58512, 34.27905,
			33.97298, 33.66691, 33.36085, 33.05478, 32.74871, 32.28965, 31.98358, 31.52451, 31.21844, 30.75931,
			30.30024, 29.84111, 29.53504, 29.07597, 28.61691, 28.15777, 27.85171, 27.39264, 26.93357, 26.47444,
			26.16837, 25.7093, 25.25017, 24.7911, 24.33203, 23.8729, 23.41383, 23.10777, 22.64863, 22.18956, 21.7305,
			21.27136, 20.8123, 20.50623, 20.04709, 19.58803, 19.28196, 18.82289, 18.36376, 18.05769, 17.59862, 17.13956,
			16.83349, 16.37436, 16.06829, 15.76222, 15.30315, 14.99709, 14.53802, 14.23195, 13.92589, 13.61982,
			13.16069, 13.00768, 12.54855, 12.24255, 11.93648, 11.63042, 11.32435, 11.01828, 10.71221, 10.40615,
			10.25308, 9.94701, 9.64101, 9.33495, 9.18188, 8.87581, 8.56975, 8.26368, 7.95761, 7.80461, 7.49854, 7.19248,
			7.03948, 6.73341, 6.58036, 6.27429, 6.12126, 5.81519, 5.66216, 5.3561, 5.20307, 5.05004, 4.74397, 4.59095,
			4.43791, 4.13185, 3.97882, 3.82579, 3.67276, 3.36669, 3.21366 };
	wvl = std::vector<double>(x, x + sizeof(x) / sizeof(x[0]));
	pde = std::vector<double>(y, y + sizeof(y) / sizeof(y[0]));
	// Apply units.
	wvl = VectorUtil::times(wvl, CLHEP::nanometer);
	pde = VectorUtil::times(pde, CLHEP::perCent);
	//
	getGainMapModel()->refresh(this);
}

std::string HamamatsuS1036211100::getName() const {
	return "HamamatsuS1036211100";
}

double HamamatsuS1036211100::getBreakdownVoltage() const {
	return 71.5 * CLHEP::volt;
}

unsigned int HamamatsuS1036211100::getNumberOfCells() const {
	return 100;
}

double HamamatsuS1036211100::getCellPitch() const {
	return 100. * CLHEP::micrometer;
}

double HamamatsuS1036211100::getThermalNoiseRate() const {
	return 799. * CLHEP::kilohertz;
}

double HamamatsuS1036211100::getDeadTime() const {
	// Minimal required current for a self-sustaining avalanche.
	double iQuench = 100.e-6 * CLHEP::ampere;
	// Dark current of the SiPM.
	double iDark = 10.e-6 * CLHEP::ampere;
	// Internal resistance of an SiPM cell (excluding quenching resistor).
	double rDiode = 1. * CLHEP::ohm;
	// Voltage equivalents.
	double vBreak = getBreakdownVoltage();
	double vQuench = vBreak + rDiode * iQuench;
	double vDark = vBreak + rDiode * iDark;
	return -getRecoveryTime() * (log(1. - vQuench / getBiasVoltage()) - log(1. - vDark / getBiasVoltage()));
}

double HamamatsuS1036211100::getRecoveryTime() const {
	return 41.1 * CLHEP::nanosecond;
}

double HamamatsuS1036211100::getCrossTalkProbability() const {
	return 16.9318 * CLHEP::perCent;
}

double HamamatsuS1036211100::getApProbLong() const {
	return 21.56655 * CLHEP::perCent;
}

double HamamatsuS1036211100::getApProbShort() const {
	return 17.64536 * CLHEP::perCent;
}

double HamamatsuS1036211100::getApTauLong() const {
	return 125.9 * CLHEP::nanosecond;
}

double HamamatsuS1036211100::getApTauShort() const {
	return 40.8 * CLHEP::nanosecond;
}

double HamamatsuS1036211100::getFillFactor() const {
	return 78.5 * CLHEP::perCent;
}

double HamamatsuS1036211100::getGainVariation() const {
	return 1. * CLHEP::perCent;
}

double HamamatsuS1036211100::getPhotonDetectionEfficiency(double wavelength) const {
	return InterpolationUtil::linear(wvl, pde, wavelength);
}
