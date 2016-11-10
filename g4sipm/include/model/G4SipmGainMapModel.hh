/*
 * G4SipmGainMapModel.hh
 *
 * @created Apr 18, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMGAINMAPMODEL_HH_
#define G4SIPMGAINMAPMODEL_HH_

#include "G4SipmCellId.hh"

class G4SipmModel;

/**
 * Gain map model base class for the G4Sipm model. Measurements did reveal a non-constant gain of the SiPM cells.
 * This automatically contributes to the width of the finger spectra recorded with an QDC.
 *
 * TODO: lazy initialization via the refresh mechanism is too dangerous.
 */
class G4SipmGainMapModel {
public:
	/**
	 * Constructor.
	 */
	G4SipmGainMapModel();
	virtual ~G4SipmGainMapModel();

	/**
	 * Refresh mechanism.
	 *
	 * @param model - the SiPM model.
	 */
	virtual void refresh(const G4SipmModel* const model) = 0;
	/**
	 * @return bool - true if a refresh is required.
	 */
	virtual bool needsRefresh() = 0;
	/**
	 * @param cellId - the id of the cell.
	 * @return double - the gain in percent.
	 */
	virtual double getGain(const G4SipmCellId cellId) const = 0;
};

#endif /* G4SIPMGAINMAPMODEL_HH_ */
