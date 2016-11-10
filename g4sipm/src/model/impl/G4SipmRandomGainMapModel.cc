/*
 * RandomGainMapModel.cc
 *
 * @created Apr 18, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/impl/G4SipmRandomGainMapModel.hh"

#include <CLHEP/Random/RandGauss.h>

#include "model/G4SipmModel.hh"

G4SipmRandomGainMapModel::G4SipmRandomGainMapModel() :
		G4SipmGainMapModel(), model(NULL) {
	//
}

void G4SipmRandomGainMapModel::refresh(const G4SipmModel* const _model) {
	model = _model;
}

bool G4SipmRandomGainMapModel::needsRefresh() {
	return model == NULL;
}

double G4SipmRandomGainMapModel::getGain(const G4SipmCellId) const {
	return CLHEP::RandGauss::shoot(1., model->getGainVariation());
}
