/*
 * Housing.cc
 *
 * @date Mar 14, 2012
 * @author Andreas Kuensken, Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "housing/G4SipmHousing.hh"

#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>

#include "MaterialFactory.hh"

G4SipmHousing::G4SipmHousing(G4Sipm* _sipm) :
		Placeable(), sipm(_sipm) {
	setName("g4sipmHousing");
	windowDx = windowDy = packageDx = packageDy = sipm->getModel()->getPitch();
	windowDz = sipm->getModel()->getWindowThickness();
	packageDz = 2. * sipm->getModel()->getThickness() + windowDz;
	containerMaterial = MaterialFactory::getInstance()->getAir();
	windowMaterial = sipm->getModel()->getWindowMaterial();
	packageMaterial = MaterialFactory::getInstance()->getBoronCarbideCeramic();
}

G4SipmHousing::~G4SipmHousing() {
	delete sipm;
}

G4LogicalVolume* G4SipmHousing::buildPackage(G4LogicalVolume* containerLv) {
	G4Box* ceramic = new G4Box("ceramic", packageDx / 2., packageDy / 2., packageDz / 2.);
	G4LogicalVolume* packageLv = new G4LogicalVolume(ceramic, packageMaterial, "packageLv");
	G4Colour grey = G4Color::Grey();
	packageLv->SetVisAttributes(new G4VisAttributes(G4Color(grey.GetRed(), grey.GetGreen(), grey.GetBlue(), 0.6)));
	G4ThreeVector packagePos(0, 0, getDz() / 2. - packageDz / 2.);
	new G4PVPlacement(NULL, packagePos, packageLv, "packagePv", containerLv, false, 0, false);
	return packageLv;
}

G4LogicalVolume* G4SipmHousing::buildWindow(G4LogicalVolume* packageLv) {
	G4Box* windowSolid = new G4Box("windowSolid", windowDx / 2., windowDy / 2., windowDz / 2.);
	G4LogicalVolume* windowLv = new G4LogicalVolume(windowSolid, windowMaterial, "windowLv");
	G4Colour blue = G4Color::Blue();
	windowLv->SetVisAttributes(new G4VisAttributes(G4Color(blue.GetRed(), blue.GetGreen(), blue.GetBlue(), 0.3)));
	G4ThreeVector windowPos(0, 0, packageDz / 2. - windowDz / 2.);
	new G4PVPlacement(NULL, windowPos, windowLv, "windowPv", packageLv, false, 0, false);
	return windowLv;
}

G4LogicalVolume* G4SipmHousing::build() {
	// Create container.
	G4Box* container = new G4Box("container", getDx() / 2., getDy() / 2., getDz() / 2.);
	G4LogicalVolume * containerLv = new G4LogicalVolume(container, containerMaterial, "containerLv");
	containerLv->SetVisAttributes(G4VisAttributes::Invisible);
	// Create the individual parts of the housing.
	G4LogicalVolume* packageLv = buildPackage(containerLv);
	buildWindow(packageLv);
	// Create SiPM.
	new G4PVPlacement(NULL, getSipmPosition(), getSipm()->build(), "sipmPv", packageLv, false, 0, false);
	return containerLv;
}

G4Material* G4SipmHousing::getContainerMaterial() const {
	return containerMaterial;
}

double G4SipmHousing::getPackageDx() const {
	return packageDx;
}

double G4SipmHousing::getPackageDy() const {
	return packageDy;
}

double G4SipmHousing::getPackageDz() const {
	return packageDz;
}

G4Material* G4SipmHousing::getPackageMaterial() const {
	return packageMaterial;
}

G4Sipm* G4SipmHousing::getSipm() const {
	return sipm;
}

double G4SipmHousing::getWindowDx() const {
	return windowDx;
}

double G4SipmHousing::getWindowDy() const {
	return windowDy;
}

double G4SipmHousing::getWindowDz() const {
	return windowDz;
}

double G4SipmHousing::getDx() const {
	return packageDx;
}

double G4SipmHousing::getDy() const {
	return packageDy;
}

double G4SipmHousing::getDz() const {
	return packageDz;
}

G4ThreeVector G4SipmHousing::getSipmPosition() const {
	return G4ThreeVector(0., 0., getPackageDz() / 2. - getWindowDz() - getSipm()->getModel()->getThickness() / 2.);
}

void G4SipmHousing::setContainerMaterial(G4Material* _containerMaterial) {
	containerMaterial = _containerMaterial;
}

void G4SipmHousing::setPackageDx(double _packageDx) {
	packageDx = _packageDx;
}

void G4SipmHousing::setPackageDy(double _packageDy) {
	packageDy = _packageDy;
}

void G4SipmHousing::setPackageDz(double _packageDz) {
	packageDz = _packageDz;
}

void G4SipmHousing::setPackageMaterial(G4Material* _packageMaterial) {
	packageMaterial = _packageMaterial;
}

void G4SipmHousing::setWindowDx(double _windowDx) {
	windowDx = _windowDx;
}

void G4SipmHousing::setWindowDy(double _windowDy) {
	windowDy = _windowDy;
}

void G4SipmHousing::setWindowDz(double _windowDz) {
	windowDz = _windowDz;
}

void G4SipmHousing::setWindowMaterial(G4Material* _windowMaterial) {
	windowMaterial = _windowMaterial;
}

G4Material* G4SipmHousing::getWindowMaterial() const {
	return windowMaterial;
}
