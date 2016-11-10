/*
 * CellFireController.cc
 *
 * @created Apr 11, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "digi/G4SipmCellFireController.hh"

#include "G4SipmUiMessenger.hh"

G4SipmCellFireController::G4SipmCellFireController(G4SipmModel* sipmModel, double t0) :
		model(sipmModel) {
	unsigned int nCells = model->getNumberOfCells();
	for (unsigned int i = 0; i < nCells; i++) {
		cellIdToTimeMap[i] = t0;
	}
	model->getGainMapModel()->refresh(model);
}

G4SipmCellFireController::~G4SipmCellFireController() {
	cellIdToTimeMap.clear();
}

bool G4SipmCellFireController::fire(G4SipmDigi* d) {
	double& cellTime = cellIdToTimeMap[d->getCellId()];
	// Check timing.
	if (G4SipmUiMessenger::getInstance()->isFilterTiming()) {
		if (cellTime + model->getDeadTime() >= d->getTime() || d->getCellId() >= model->getNumberOfCells()
				|| d->getWeight() <= 0.) {
			return false;
		}
	}
	// Determine gain from the recovery behaviour of the capacitor in the diode equivalent circuit.
	double gain = 1. - exp(-(d->getTime() - cellTime - model->getDeadTime()) / model->getRecoveryTime());
	// Update weight of the digi.
	d->setWeight(gain * model->getGain(d->getCellId()));
	// Update time.
	cellTime = d->getTime();
	// Success.
	return true;
}
