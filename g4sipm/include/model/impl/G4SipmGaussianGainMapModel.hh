/*
 * G4SipmGaussianGainMapModel.hh
 *
 * @created Apr 18, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMGAUSSIANGAINMAPMODEL_HH_
#define G4SIPMGAUSSIANGAINMAPMODEL_HH_

#include "model/G4SipmGainMapModel.hh"

#include <vector>

class G4SipmModel;

/**
 * Gain map implementation which is derived from a 2D-Gauss distribution.
 *
 * TODO: currently does not correctly reproduce the variance configured in the SiPM model.
 */
class G4SipmGaussianGainMapModel: public G4SipmGainMapModel {
private:
	std::vector<double> gain;

public:
	/**
	 * Constructor.
	 */
	G4SipmGaussianGainMapModel();

	virtual void refresh(const G4SipmModel* const model);
	virtual bool needsRefresh();
	virtual double getGain(const G4SipmCellId cellId) const;
};

#endif /* G4SIPMGAUSSIANGAINMAPMODEL_HH_ */
