/*
 * SipmVoltageTraceModel.cc
 *
 * @created Apr 19, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/G4SipmVoltageTraceModel.hh"

#include <math.h>

G4SipmVoltageTraceModel::G4SipmVoltageTraceModel() {
	//
}

G4SipmVoltageTraceModel::~G4SipmVoltageTraceModel() {
	//
}

double G4SipmVoltageTraceModel::singleExponential(const double t, const double gain) const {
	if (t < 0.) {
		return 0.;
	}
	if (t < getTauRise()) {
		return getAmplitude() * gain * (t / getTauRise());
	}
	return getAmplitude() * gain * exp(-(t - getTauRise()) / getTauFall());
}

double G4SipmVoltageTraceModel::doubleExponential(const double t, const double gain) const {
	if (t < 0.) {
		return 0.;
	}
	return getAmplitude() * gain * (1. - exp(-t / getTauRise())) * exp(-t / getTauFall());
}

double G4SipmVoltageTraceModel::pulse(const double t, const double gain) const {
	return doubleExponential(t, gain);
}

