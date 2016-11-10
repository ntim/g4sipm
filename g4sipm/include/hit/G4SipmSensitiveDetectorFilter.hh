/*
 * G4SipmSensitiveDetectorFilter.hh
 *
 * @date Feb 10, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMSENSITIVEDETECTORFILTER_HH_
#define G4SIPMSENSITIVEDETECTORFILTER_HH_

#include <G4VSDFilter.hh>
#include <G4MaterialPropertyVector.hh>

#include "G4Sipm.hh"

/**
 * The detector filter for the G4SipmSensitiveDetector. Handles the loss in sensitivity by
 * the fill factor and the photon detection efficiency.
 */
class G4SipmSensitiveDetectorFilter: public G4VSDFilter {
private:
	const G4Sipm* sipm;
	G4MaterialPropertyVector* airRindex;
	G4MaterialPropertyVector* windowRindex;
	G4MaterialPropertyVector* sipmRindex;

protected:
	/**
	 * Dices the acceptance according to the PDE.
	 *
	 * @param eKin - the energy of the photon.
	 * @param theta - the angle of incidence of the photon.
	 * @return bool - true if the photon should be accepted.
	 */
	bool acceptPde(double eKin, double theta) const;
	/**
	 * Checks if the photon hits dead space between the single cells.
	 *
	 * @param x - the x position of the photon.
	 * @param y - the y position of the photon.
	 * @return bool - true if the photon should be accepted.
	 */
	bool acceptGeometry(double x, double y) const;

public:
	/**
	 * Constructor.
	 *
	 * @param sipm - the SiPM.
	 */
	G4SipmSensitiveDetectorFilter(const G4Sipm* sipm);
	virtual ~G4SipmSensitiveDetectorFilter();

	virtual G4bool Accept(const G4Step*) const;

};

#endif /* G4SIPMSENSITIVEDETECTORFILTER_HH_ */
