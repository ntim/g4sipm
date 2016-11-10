/*
 * HamamatsuCeramicHousing.cc
 *
 * @created Apr 29, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "housing/impl/HamamatsuCeramicHousing.hh"

#include <CLHEP/Units/SystemOfUnits.h>
#include <G4Tubs.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

#include "MaterialFactory.hh"

HamamatsuCeramicHousing::HamamatsuCeramicHousing(G4Sipm* _sipm) :
		G4SipmHousing(_sipm), ratio(_sipm->getModel()->getPitch() / (3. * CLHEP::mm)) {
	setPackageDx(3.55 * CLHEP::mm * ratio + _sipm->getModel()->getPitch());
	setPackageDy(2.9 * CLHEP::mm * ratio + _sipm->getModel()->getPitch());
	setPackageDz(2. * CLHEP::mm);
	setWindowDx(getPackageDx() - 1. * CLHEP::mm * ratio);
	setWindowDy(getPackageDy() - 1. * CLHEP::mm * ratio);
	setWindowDz(0.45 * CLHEP::mm);
}

G4LogicalVolume* HamamatsuCeramicHousing::build() {
	G4LogicalVolume* containerLv = G4SipmHousing::build();
	// Build connectors.
	G4VSolid* connector = new G4Tubs("connector", 0., .46 * CLHEP::mm / 2., 6. * ratio / 2., 0, CLHEP::twopi);
	G4LogicalVolume* connectorLv = new G4LogicalVolume(connector, MaterialFactory::getInstance()->getCopper(),
			"connectorLv");
	connectorLv->SetVisAttributes(new G4VisAttributes(G4Colour::Yellow()));
	G4ThreeVector anodePos(2. * CLHEP::mm * ratio, 1.27 * CLHEP::mm * ratio,
			-getDz() / 2. + 6. * CLHEP::mm * ratio / 2.);
	G4ThreeVector kathodePos(anodePos.x(), -anodePos.y(), anodePos.z());
	new G4PVPlacement(NULL, anodePos, connectorLv, "anodePv", containerLv, false, 0, true);
	new G4PVPlacement(NULL, kathodePos, connectorLv, "kathodePv", containerLv, false, 1, true);
	//
	return containerLv;
}

G4ThreeVector HamamatsuCeramicHousing::getSipmPosition() const {
	G4ThreeVector pos = G4SipmHousing::getSipmPosition();
	return pos + G4ThreeVector(0.33 * CLHEP::mm * ratio / 2., 0, 0);
}

double HamamatsuCeramicHousing::getDz() const {
	// Adjust depth for the connector length.
	return getPackageDz() + 6. * CLHEP::mm * ratio;
}
