/*
 * HamamatsuCeramicHousing.hh
 *
 * @created Apr 29, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUCERAMICHOUSING_HH_
#define HAMAMATSUCERAMICHOUSING_HH_

#include "housing/G4SipmHousing.hh"

/**
 * Hamamatsu ceramic housing implementation. Relations are correct for the 3x3 model.
 */
class HamamatsuCeramicHousing : public G4SipmHousing{
private:
	const double ratio;

public:
	/**
	 * Constructor.
	 *
	 * @param sipm - the SiPM.
	 */
	HamamatsuCeramicHousing(G4Sipm* sipm);

	virtual G4LogicalVolume* build();
	virtual G4ThreeVector getSipmPosition() const;
	virtual double getDz() const;
};
#endif /* HAMAMATSUCERAMICHOUSING_HH_ */
