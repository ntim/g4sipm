/*
 * G4SipmRandomGainMapModel.hh
 *
 * @created Apr 18, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMRANDOMGAINMAPMODEL_HH_
#define G4SIPMRANDOMGAINMAPMODEL_HH_

#include "model/G4SipmGainMapModel.hh"

#include <vector>

class G4SipmModel;

/**
 * Gain map model which sets a random gain to each cell.
 */
class G4SipmRandomGainMapModel: public G4SipmGainMapModel {
private:
	std::vector<double> gain;
	const G4SipmModel* model;

public:
	/**
	 * Constructor.
	 */
	G4SipmRandomGainMapModel();

	virtual void refresh(const G4SipmModel* const model);
	virtual bool needsRefresh();
	virtual double getGain(const G4SipmCellId cellId) const;
};

#endif /* G4SIPMRANDOMGAINMAPMODEL_HH_ */
