/*
 * G4Sipm.hh
 *
 * @created Apr 22, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPM_HH_
#define G4SIPM_HH_

#include "Placeable.hh"
#include "model/G4SipmModel.hh"

/**
 * Typedef for identifying G4Sipm instances.
 */
typedef unsigned int G4SipmId;

/**
 * Placeable representing a for the SiPM. Has an unique Id. Holds an instance of G4SipmModel for the sensitivity
 * simulation and a G4SipmPulseShapeModel for the simulation of the electronics.
 */
class G4Sipm: public Placeable {
private:
	static G4SipmId LAST_ID;
	G4SipmId id;
	G4SipmModel* model;

public:
	/**
	 * Constructor.
	 *
	 * @param model - the SiPM model.
	 */
	G4Sipm(G4SipmModel* model);
	virtual ~G4Sipm();

	virtual G4LogicalVolume* build();

	/**
	 * @return G4String - the unique sensitive detector name.
	 */
	G4String getSensitiveDetectorName() const;
	/**
	 * @return G4String - the unique hit collection name.
	 */
	G4String getHitCollectionName() const;
	/**
	 * @return G4String - the unique digi collection name.
	 */
	G4String getDigiCollectionName() const;
	/**
	 * @return G4String - the unique voltage trace digi collection name.
	 */
	G4String getVoltageTraceDigiCollectionName() const;
	/**
	 * @return G4String - the unique digitizer module name.
	 */
	G4String getDigitizerName() const;
	/**
	 * @return G4String - the unique voltage trace digitizer name.
	 */
	G4String getVoltageTraceDigitizerName() const;

	/**
	 * @return G4SipmId - the SiPM id.
	 */
	G4SipmId getId() const;
	/**
	 * @return G4SipmModel - the SiPM model.
	 */
	G4SipmModel* getModel() const;
};

#endif /* G4SIPM_HH_ */
