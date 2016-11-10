/*
 * HamamatsuSmdHousing.cc
 *
 * @created Apr 29, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "housing/impl/HamamatsuSmdHousing.hh"

#include <CLHEP/Units/SystemOfUnits.h>
#include <G4Box.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>

#include "MaterialFactory.hh"

using CLHEP::mm;

HamamatsuSmdHousing::HamamatsuSmdHousing(G4Sipm* _sipm) :
		G4SipmHousing(_sipm), ratio(_sipm->getModel()->getPitch() / (3.0 * CLHEP::mm)) {
	// Adapt dimensions from the data sheet.
	setPackageDx((0.425 + 0.925) * CLHEP::mm * ratio + _sipm->getModel()->getPitch());
	setPackageDy(2.0 * 0.425 * CLHEP::mm * ratio + _sipm->getModel()->getPitch());
	setWindowDx(getPackageDx());
	setWindowDy(getPackageDy());
	setPackageDz(1.45 * CLHEP::mm);
	setWindowDz(0.3 * CLHEP::mm);
}

G4LogicalVolume* HamamatsuSmdHousing::build() {
	G4LogicalVolume* containerLv = G4SipmHousing::build();
	// Build connectors.
	G4VSolid* connector = new G4Box("connector", (getPackageDx() - 0.1 * CLHEP::mm * ratio) / 2.0,
			(1.0 * CLHEP::mm * ratio) / 2.0, 0.1 * mm);
	G4LogicalVolume* connectorLv = new G4LogicalVolume(connector, MaterialFactory::getInstance()->getCopper(),
			"connectorLv");
	connectorLv->SetVisAttributes(new G4VisAttributes(G4Colour::Yellow()));
	G4ThreeVector anodePos(-0.05 * mm * ratio, (1.1 * CLHEP::mm - 1. * CLHEP::mm / 2.0) * ratio,
			-getDz() / 2.0 + 0.1 * CLHEP::mm);
	G4ThreeVector kathodePos(anodePos.x(), -anodePos.y(), anodePos.z());
	G4LogicalVolume* packageLv = containerLv->GetDaughter(0)->GetLogicalVolume();
	new G4PVPlacement(NULL, anodePos, connectorLv, "anodePv", packageLv, false, 0, true);
	new G4PVPlacement(NULL, kathodePos, connectorLv, "kathodePv", packageLv, false, 1, true);
	//
	return containerLv;
}

G4ThreeVector HamamatsuSmdHousing::getSipmPosition() const {
	G4ThreeVector pos = G4SipmHousing::getSipmPosition();
	// Apply shift in x-direction.
	return pos + G4ThreeVector((0.925 - 0.425) / 2.0 * CLHEP::mm * ratio, 0., 0.);
}
