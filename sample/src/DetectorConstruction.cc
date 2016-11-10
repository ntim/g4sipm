/*
 * DetectorConstruction.cc
 *
 * @date Sep 12, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "DetectorConstruction.hh"

#include <G4Box.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>

#include "G4Sipm.hh"
#include "MaterialFactory.hh"
#include "model/G4SipmModelFactory.hh"
#include "housing/G4SipmHousing.hh"
#include "housing/impl/HamamatsuCeramicHousing.hh"
#include "housing/impl/HamamatsuSmdHousing.hh"

DetectorConstruction::DetectorConstruction(std::string sipmModelName, std::string housingName) :
		G4VUserDetectorConstruction() {
	// Create SiPM and housing.
	G4SipmModel* model = createSipmModel(sipmModelName);
	housing = createHousing(housingName, new G4Sipm(model));
}

DetectorConstruction::~DetectorConstruction() {
	delete housing;
}

G4SipmModel* DetectorConstruction::createSipmModel(std::string name) const {
	if (name == "generic") {
		return G4SipmModelFactory::getInstance()->createGenericSipmModel();
	}
	if (name == "HamamatsuS1036211100") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS1036211100();
	}
	if (name == "HamamatsuS1036233100") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS1036233100();
	}
	if (name == "HamamatsuS10985100") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS10985100();
	}
	if (name == "HamamatsuS12651050") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS12651050();
	}
	if (name == "HamamatsuS1036233050") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS1036233050();
	}
	if (name == "HamamatsuS12573100C") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS12573100C();
	}
	if (name == "HamamatsuS12573100X") {
		return G4SipmModelFactory::getInstance()->createHamamatsuS12573100X();
	}
	return G4SipmModelFactory::getInstance()->createConfigFileModel(name);
}

G4SipmHousing* DetectorConstruction::createHousing(std::string name, G4Sipm* sipm) const {
	if (name == "ceramic") {
		return new HamamatsuCeramicHousing(sipm);
	}
	if (name == "smd") {
		return new HamamatsuSmdHousing(sipm);
	}
	if (name == "default") {
		return new G4SipmHousing(sipm);
	}
	std::cout << "G4SipmDetectorConstruction::createHousingForName(name = \"" << name
			<< "\"): housing type does not exist." << std::endl;
	throw 1;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
	G4ThreeVector worldDimension = G4ThreeVector(1. * CLHEP::m, 1. * CLHEP::m, 1. * CLHEP::m);
	// Create world volume.
	G4Box* world = new G4Box("world", worldDimension.x(), worldDimension.y(), worldDimension.z());
	G4LogicalVolume* worldLv = new G4LogicalVolume(world, MaterialFactory::getInstance()->getAir(), "worldLv", NULL,
	NULL, NULL);
	worldLv->SetVisAttributes(G4VisAttributes::Invisible);
	G4VPhysicalVolume* worldPv = new G4PVPlacement(NULL, G4ThreeVector(), worldLv, "worldPv", NULL, false, 0);
	// Set the entrance window surface of the SiPM to (0,0,0).
	housing->setPosition(G4ThreeVector(0., 0., -housing->getDz() / 2.));
	// Build SiPM.
	housing->buildAndPlace(worldPv);
	//
	return worldPv;
}

G4SipmModel* DetectorConstruction::getSipmModel() const {
	return housing->getSipm()->getModel();
}

G4SipmHousing* DetectorConstruction::getSipmHousing() const {
	return housing;
}
