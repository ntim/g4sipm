/*
 * Placeable.hh
 *
 * @date Dec 14, 2010
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef PLACEABLE_HH_
#define PLACEABLE_HH_

#include <globals.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>

/**
 * This abstract class represents a placeable.
 */
class Placeable {
private:
	G4String name;
	G4ThreeVector position;
	G4RotationMatrix* rotation;

public:
	/**
	 * Constructor.
	 */
	Placeable();
	virtual ~Placeable();

	/**
	 * Builds the geometry.
	 *
	 * @return G4LogicalVolume* - the build logical volume.
	 */
	virtual G4LogicalVolume* build() = 0;

	/**
	 * Builds the logical volume and places it.
	 *
	 * @param parent - the parent.
	 * @return G4VPhysicalVolume* - the physical volume.
	 */
	virtual G4VPhysicalVolume* buildAndPlace(G4VPhysicalVolume* parent);

	/**
	 * Builds the logical volume and places it.
	 *
	 * @param parent - the parent.
	 * @param position - the position.
	 * @param rotation - the rotation.
	 * @return G4VPhysicalVolume* - the physical volume.
	 */
	virtual G4VPhysicalVolume* buildAndPlace(G4VPhysicalVolume* parent, G4ThreeVector position,
			G4RotationMatrix* rotation = NULL);

	/**
	 * Builds the logical volume and places it relative to a sibling in the volume hierarchy.
	 *
	 * @param sibling - the sibling.
	 * @param position - the position.
	 * @param rotation - the rotation.
	 * @return G4VPhysicalVolume* - the physical volume.
	 */
	virtual G4VPhysicalVolume* buildAndPlaceRelativeTo(G4VPhysicalVolume* sibling, G4ThreeVector position,
			G4RotationMatrix* rotation = NULL);

	/**
	 * @return G4String - the name.
	 */
	G4String getName() const;

	/**
	 * @param name - the new name to set.
	 */
	void setName(G4String name);

	/**
	 * @return G4ThreeVector - the position.
	 */
	G4ThreeVector getPosition() const;

	/**
	 * @param position - the new position to set.
	 */
	void setPosition(G4ThreeVector position);

	/**
	 * @return G4RotationMatrix - the rotation matrix.
	 */
	G4RotationMatrix* getRotation() const;

	/**
	 * @param rotation - the new rotation to set.
	 */
	void setRotation(G4RotationMatrix* rotation);

};

#endif /* PLACEABLE_HH_ */
