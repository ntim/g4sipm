/*
 * DetectorConstruction.hh
 *
 * @date Sep 12, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef DETECTORCONSTRUCTION_HH_
#define DETECTORCONSTRUCTION_HH_

#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VUserDetectorConstruction.hh>

#include "G4Sipm.hh"
#include "housing/G4SipmHousing.hh"

/**
 * User detector construction. Places a single SiPM in the world.
 */
class DetectorConstruction: public G4VUserDetectorConstruction {
private:
	G4SipmHousing* housing;
	/**
	 * @param name - the name of the model.
	 * @return G4SipmModel - the new instance.
	 */
	G4SipmModel* createSipmModel(std::string name) const;
	/**
	 * @param name - the name of the housing.
	 * @param sipm - the SiPM instance.
	 * @return G4SipmHousing - the new instance.
	 */
	G4SipmHousing* createHousing(std::string name, G4Sipm* sipm) const;

public:
	/**
	 * Constructor.
	 *
	 * @param modelName - the model name (see G4SipmModelFactory).
	 * @param housingName - the name of the housing ("ceramic", "smd", "plain").
	 */
	DetectorConstruction(std::string modelName, std::string housingName);
	virtual ~DetectorConstruction();

	G4VPhysicalVolume* Construct();

	/**
	 * @return G4SipmModel - the SiPM model.
	 */
	G4SipmModel* getSipmModel() const;
	/**
	 * @return G4SipmHousing - the housing instance.
	 */
	G4SipmHousing* getSipmHousing() const;

};

#endif /* G4SIPMDETECTORCONSTRUCTION_HH_ */
