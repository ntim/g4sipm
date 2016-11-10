/*
 * G4SipmCellFireController.hh
 *
 * @created Apr 11, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMCELLFIRECONTROLLER_HH_
#define G4SIPMCELLFIRECONTROLLER_HH_

#include <vector>
#include <map>

#include "model/G4SipmModel.hh"
#include "digi/G4SipmDigi.hh"

/**
 * Encapsulates the cell firing mechanism. Should be recreated for each run.
 */
class G4SipmCellFireController {
protected:
	G4SipmModel* model;
	std::map<G4SipmCellId, double> cellIdToTimeMap;

public:
	/**
	 * Constructor.
	 *
	 * @param model - the underlying SiPM model.
	 * @param t0 - the initial time of all SiPM cells.
	 */
	G4SipmCellFireController(G4SipmModel* model, double t0 = 0.);
	virtual ~G4SipmCellFireController();

	/**
	 * Checks whether the trigger can be successfully made or not.
	 *
	 * @param d - the cell trigger digi.
	 * @return bool - true if the trigger could be fired, false if not.
	 */
	virtual bool fire(G4SipmDigi* d);
};

#endif /* G4SIPMCELLFIRECONTROLLER_HH_ */
