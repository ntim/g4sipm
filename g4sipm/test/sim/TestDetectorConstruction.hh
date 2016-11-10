/*
 * TestDetectorConstruction.hh
 *
 * @date Mar 23, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef TESTDETECTORCONSTRUCTION_HH_
#define TESTDETECTORCONSTRUCTION_HH_

#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>
#include <G4GeometryManager.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SolidStore.hh>
#include <G4LogicalVolume.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4PVPlacement.hh>
#include <G4Box.hh>

#include <CLHEP/Units/SystemOfUnits.h>

#include "MaterialFactory.hh"
#include "G4Sipm.hh"
#include "model/G4SipmModel.hh"

class TestDetectorConstruction: public G4VUserDetectorConstruction {
private:
	G4SipmModel* model;
	G4Sipm* sipm;

public:
	TestDetectorConstruction(G4SipmModel* _model) :
			G4VUserDetectorConstruction(), model(_model), sipm(NULL) {
		//
	}

	virtual ~TestDetectorConstruction() {
		delete sipm;
	}

	virtual G4VPhysicalVolume* Construct() {
		// Cleanup old geometry.
		G4GeometryManager::GetInstance()->OpenGeometry();
		G4PhysicalVolumeStore::GetInstance()->Clean();
		G4LogicalVolumeStore::GetInstance()->Clean();
		G4SolidStore::GetInstance()->Clean();
		// Create world volume.
		G4ThreeVector worldDimension = G4ThreeVector(1. * CLHEP::cm, 1. * CLHEP::cm, 1. * CLHEP::cm);
		// Create world volume.
		G4Box* world = new G4Box("world", worldDimension.x(), worldDimension.y(), worldDimension.z());
		G4LogicalVolume* worldLv = new G4LogicalVolume(world, MaterialFactory::getInstance()->getAir(), "worldLv", NULL,
		NULL, NULL);
		G4VPhysicalVolume* worldPv = new G4PVPlacement(NULL, G4ThreeVector(), worldLv, "worldPv", NULL, false, 0);
		// Create SiPM.
		if (sipm) {
			delete sipm;
		}
		sipm = new G4Sipm(model);
		sipm->buildAndPlace(worldPv);
		//
		return worldPv;
	}

	G4Sipm* getSipm() const {
		return sipm;
	}

};

#endif /* TESTDETECTORCONSTRUCTION_HH_ */
