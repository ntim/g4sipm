/*
 * GaussianGainMapModel.cc
 *
 * @created Apr 18, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/impl/G4SipmGaussianGainMapModel.hh"

#include <CLHEP/Random/RandGauss.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "model/G4SipmModel.hh"

G4SipmGaussianGainMapModel::G4SipmGaussianGainMapModel() :
		G4SipmGainMapModel() {
	//
}

void G4SipmGaussianGainMapModel::refresh(const G4SipmModel* const model) {
	// Compute 2d gauss parameters.
	const double sigmaGain = model->getGainVariation();
	const double nSigma = 3;
	// The radial gauss is fully defined by pinning it to two known points:
	// f(0,0) = 1. + nSigma * sigmaGain
	// f(d,d) = 1. - nSigma * sigmaGain
	// with d = half pitch of the SiPM.
	const double a = 1. + nSigma * sigmaGain;
	const double sigma = ((model->getPitch() - model->getCellPitch()) / 2.)
			/ sqrt(-log((1. - nSigma * sigmaGain) / (1. + nSigma * sigmaGain)));
	// Compute gains.
	gain.clear();
	for (G4SipmCellId i = 0; i < model->getNumberOfCells(); i++) {
		CLHEP::Hep2Vector pos = model->getCellPosition(i);
		gain.push_back(a * exp(-pos.mag2() / (2. * sigma * sigma)));
	}
}

bool G4SipmGaussianGainMapModel::needsRefresh() {
	return gain.empty();
}

double G4SipmGaussianGainMapModel::getGain(const G4SipmCellId cellId) const {
	if (cellId < gain.size()) {
		return gain.at(cellId);
	}
	std::cerr << "G4SipmRandomGainMapModel::getGain(): cellId " << cellId << " does not exist." << std::endl;
	throw 1;
}
