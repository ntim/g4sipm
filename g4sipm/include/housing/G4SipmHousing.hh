/*
 * Housing.hh
 *
 * @date Mar 14, 2012
 * @author Andreas Kuensken, Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMHOUSING_HH
#define G4SIPMHOUSING_HH

#include <G4VPhysicalVolume.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>

#include "G4Sipm.hh"

/**
 * Representation of the encapsulating housing of the sensitive area of a G4Sipm. Also creates a window in front
 * of the sensitive area which is necessary to reproduce the correct angular behavior of the sensitivity of the
 * SiPM.
 */
class G4SipmHousing: public Placeable {
private:
	G4Sipm* sipm;
	double windowDx;
	double windowDy;
	double windowDz;
	double packageDx;
	double packageDy;
	double packageDz;
	G4Material* containerMaterial;
	G4Material* windowMaterial;
	G4Material* packageMaterial;

protected:
	/**
	 * @param containerMaterial - the container material to set.
	 */
	void setContainerMaterial(G4Material* containerMaterial);
	/**
	 * @param packageDx - the x dimension of the package to set.
	 */
	void setPackageDx(double packageDx);
	/**
	 * @param packageDy - the y dimension of the package to set.
	 */
	void setPackageDy(double packageDy);
	/**
	 * @param packageDz - the z dimension of the package to set.
	 */
	void setPackageDz(double packageDz);
	/**
	 * @param packageMaterial - the container material to set.
	 */
	void setPackageMaterial(G4Material* packageMaterial);
	/**
	 * @param windowDx - the x dimension of the window to set.
	 */
	void setWindowDx(double windowDx);
	/**
	 * @param windowDy - the y dimension of the window to set.
	 */
	void setWindowDy(double windowDy);
	/**
	 * @param windowDz - the z dimension of the window to set.
	 */
	void setWindowDz(double windowDz);
	/**
	 * @param windowMaterial - the container material to set.
	 */
	void setWindowMaterial(G4Material* windowMaterial);
	/**
	 * Builds the ceramic package.
	 *
	 * @param containerLv - the container logical volume.
	 * @return G4LogicalVolume * - the logical volume of the package.
	 */
	virtual G4LogicalVolume* buildPackage(G4LogicalVolume* containerLv);
	/**
	 * Builds the window into the ceramic.
	 *
	 * @param packageLv - the ceramic logical volume.
	 * @return G4LogicalVolume * - the logical volume of the package.
	 */
	virtual G4LogicalVolume* buildWindow(G4LogicalVolume* packageLv);

public:
	/**
	 * Constructor.
	 *
	 * @param sipm - the SiPM.
	 */
	G4SipmHousing(G4Sipm* sipm);
	virtual ~G4SipmHousing();

	virtual G4LogicalVolume* build();
	/**
	 * @return double - the total x dimension.
	 */
	virtual double getDx() const;
	/**
	 * @return double - the total y dimension.
	 */
	virtual double getDy() const;
	/**
	 * @return double - the total z dimension.
	 */
	virtual double getDz() const;
	/**
	 * @return G4ThreeVector - the position of the SiPM.
	 */
	virtual G4ThreeVector getSipmPosition() const;

	/**
	 * @return G4Sipm - the SiPM instance.
	 */
	G4Sipm* getSipm() const;
	/**
	 * @return double - the package x dimension.
	 */
	double getPackageDx() const;
	/**
	 * @return double - the package y dimension.
	 */
	double getPackageDy() const;
	/**
	 * @return double - the package z dimension.
	 */
	double getPackageDz() const;
	/**
	 * @return double - the window x dimension.
	 */
	double getWindowDx() const;
	/**
	 * @return double - the window y dimension.
	 */
	double getWindowDy() const;
	/**
	 * @return double - the window z dimension.
	 */
	double getWindowDz() const;
	/**
	 * @return G4Material - the material of the container (Air).
	 */
	G4Material* getContainerMaterial() const;
	/**
	 * @return G4Material - the material of the package.
	 */
	G4Material* getPackageMaterial() const;
	/**
	 * @return G4Material - the material of the window.
	 */
	G4Material* getWindowMaterial() const;
};

#endif /* G4SIPMHOUSING_HH */
