/*
 * Placeable.cc
 *
 * @date Mar 14, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "Placeable.hh"

#include <G4PVPlacement.hh>

Placeable::Placeable() :
		name(""), position(G4ThreeVector(0, 0, 0)), rotation(NULL) {
	//
}

Placeable::~Placeable() {
	//
}

G4VPhysicalVolume* Placeable::buildAndPlace(G4VPhysicalVolume* parent) {
	return new G4PVPlacement(rotation, position, name + "Pv", build(), parent, false, 0, false);
}

G4VPhysicalVolume* Placeable::buildAndPlace(G4VPhysicalVolume* parent, G4ThreeVector _position,
		G4RotationMatrix* _rotation) {
	setPosition(_position);
	setRotation(_rotation);
	return new G4PVPlacement(rotation, position, name + "Pv", build(), parent, false, 0, false);
}

G4VPhysicalVolume* Placeable::buildAndPlaceRelativeTo(G4VPhysicalVolume* sibling, G4ThreeVector _position,
		G4RotationMatrix* _rotation) {
	G4LogicalVolume* parent = sibling->GetMotherLogical();
	// Transform position and rotation to the parent frame.
	G4ThreeVector newPosition = _position + sibling->GetObjectTranslation();
	G4RotationMatrix* newRotation = _rotation;
	if (_rotation) {
		*newRotation = *(sibling->GetObjectRotation());
	}
	setPosition(newPosition);
	setRotation(newRotation);
	return new G4PVPlacement(newRotation, newPosition, build(), name + "Pv", parent, false, 0, false);
}

G4String Placeable::getName() const {
	return name;
}

void Placeable::setName(G4String _name) {
	name = _name;
}

G4ThreeVector Placeable::getPosition() const {
	return position;
}

void Placeable::setPosition(G4ThreeVector _position) {
	position = _position;
}

G4RotationMatrix* Placeable::getRotation() const {
	return rotation;
}

void Placeable::setRotation(G4RotationMatrix* _rotation) {
	if (rotation) {
		delete rotation;
	}
	rotation = _rotation;
}
