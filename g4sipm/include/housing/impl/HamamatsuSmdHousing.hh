/*
 * HamamatsuSmdHousing.hh
 *
 * @created Apr 29, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUSMDHOUSING_HH_
#define HAMAMATSUSMDHOUSING_HH_

#include "housing/G4SipmHousing.hh"

/**
 * Hamamatsu SMD housing type implementation. Reference implementation for the 3x3 mm² type.
 * For smaller/larger devices, the same size ratios are applied.
 */
class HamamatsuSmdHousing: public G4SipmHousing {
private:
	const double ratio;

public:
	/**
	 * Constructor.
	 *
	 * @param sipm - the SiPM.
	 */
	HamamatsuSmdHousing(G4Sipm* sipm);

	virtual G4LogicalVolume* build();
	virtual G4ThreeVector getSipmPosition() const;
};

#endif /* HAMAMATSUSMDHOUSING_HH_ */
