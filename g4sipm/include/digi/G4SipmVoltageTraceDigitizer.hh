/*
 * G4SipmVoltageTraceDigitizer.hh
 *
 * @created Apr 19, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMVOLTAGETRACEDIGITIZER_HH_
#define G4SIPMVOLTAGETRACEDIGITIZER_HH_

#include <G4VDigitizerModule.hh>

#include "G4Sipm.hh"
#include "digi/G4SipmDigi.hh"
#include "digi/G4SipmVoltageTraceDigi.hh"

/**
 * Digitizer module converting the list of cell triggers (G4SipmDigi) into a single voltage trace (G4SipmVoltageTraceDigi).
 */
class G4SipmVoltageTraceDigitizer: public G4VDigitizerModule {
	/**
	 * G4Sipm is a friend class to allow a proper set to NULL when the SiPM instance is destroyed.
	 */
	friend class G4Sipm;
private:
	G4Sipm* sipm;

protected:
	/**
	 * @param sipm - the SiPM to be set.
	 */
	void setSipm(G4Sipm* sipm);

public:
	/**
	 * Constructor. Registers itself to Geant4.
	 *
	 * @param sipm - the SiPM instance.
	 */
	G4SipmVoltageTraceDigitizer(G4Sipm* sipm);
	virtual ~G4SipmVoltageTraceDigitizer();

	virtual void Digitize();
	/**
	 * @return G4SipmDigiCollection - the digi collection.
	 */
	const G4SipmDigiCollection* getDigiCollection();
	/**
	 * @return G4Sipm - the SiPM instance.
	 */
	G4Sipm* getSipm() const;
};

#endif /* G4SIPMVOLTAGETRACEDIGITIZER_HH_ */
