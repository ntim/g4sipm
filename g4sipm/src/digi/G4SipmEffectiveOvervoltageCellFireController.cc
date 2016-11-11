/*
 * G4SipmEffectiveOvervoltageCellFireController.cc
 *
 * @date Feb 17, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <digi/G4SipmEffectiveOvervoltageCellFireController.hh>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Random/RandFlat.h>

#include "G4SipmUiMessenger.hh"

G4SipmEffectiveOvervoltageCellFireController::G4SipmEffectiveOvervoltageCellFireController(G4SipmModel* _model,
		double t0) :
		G4SipmCellFireController(_model, t0) {
	effectiveOvervoltage = model->getOverVoltage();
	shuntResistor = 50. * CLHEP::ohm;
	quenchingResistor = 151.2e3 * CLHEP::ohm;
	tLast = t0;
	nCoincident = 1;
	// Recharge time constant for a 50 Ohm shunt resistor and Hamamatsu 3x3 50 mu pitch: 30.12 * CLHEP::nanosecond.
	tau = G4SipmUiMessenger::getInstance()->getShuntresistorRecoveryTime();
	//	vu = 0.0002104984 * CLHEP::volt;
	//	vu = 0.00033 * CLHEP::volt;
	vu = model->getOverVoltage() * shuntResistor / (quenchingResistor + shuntResistor);
}

inline double rechargeOvervoltage(double dt, double vEff, double vOv, double tau) {
	double t1 = -tau * log(1. - vEff / vOv);
	return vOv * (1. - exp(-(t1 + dt) / tau));
}

bool G4SipmEffectiveOvervoltageCellFireController::fire(G4SipmDigi* d) {
	double& cellTime = cellIdToTimeMap[d->getCellId()];
	// Check timing.
	if (G4SipmUiMessenger::getInstance()->isFilterTiming()) {
		if (cellTime + model->getDeadTime() >= d->getTime() || d->getCellId() >= model->getNumberOfCells()
				|| d->getWeight() <= 0.) {
			return false;
		}
	}
	// Account for the PDE drop linearly with the over voltage.
	if (d->getType() == PHOTON) {
		if (CLHEP::RandFlat::shoot() > effectiveOvervoltage / model->getOverVoltage()) {
			return false;
		}
	}
	// Recharge effective over voltage.
	effectiveOvervoltage = rechargeOvervoltage(d->getTime() - tLast /* - model->getDeadTime()*/, effectiveOvervoltage,
			model->getOverVoltage(), tau);
	// Determine gain from the recovery behaviour of the capacitor in the diode equivalent circuit.
	double gain = 1. - exp(-(d->getTime() - cellTime - model->getDeadTime()) / model->getRecoveryTime());
	// Update weight of the digi and include gain variation.
	d->setWeight(gain * effectiveOvervoltage / model->getOverVoltage() * model->getGain(d->getCellId()));
	// If this pulse is a coincident pulse do not decrease effective overvoltage just yet.
	if (d->getTime() <= tLast + model->getDeadTime()) {
		nCoincident++;
	} else {
		// Reduce effective over voltage.
		effectiveOvervoltage -= ((double) nCoincident) * vu * effectiveOvervoltage / model->getOverVoltage()
				* d->getWeight();
		effectiveOvervoltage = std::max(effectiveOvervoltage, 0.);
		nCoincident = 1;
	}
	// Update time.
	cellTime = d->getTime();
	tLast = d->getTime();
	// Success.
	return true;
}
