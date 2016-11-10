/*
 * G4Sipm.cc
 *
 * @created Apr 22, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "G4Sipm.hh"

#include <G4Box.hh>
#include <G4SDManager.hh>
#include <G4VisAttributes.hh>
#include <G4DigiManager.hh>

#include "hit/G4SipmSensitiveDetector.hh"
#include "digi/G4SipmDigitizer.hh"
#include "digi/G4SipmVoltageTraceDigitizer.hh"

G4SipmId G4Sipm::LAST_ID = 0;

G4Sipm::G4Sipm(G4SipmModel* _model) :
		Placeable(), model(_model) {
	setName("g4sipm");
	// Nifty counter.
	id = LAST_ID++;
}

G4Sipm::~G4Sipm() {
	// Deactivate sensitive detector.
	if (G4SDManager::GetSDMpointer()->FindSensitiveDetector(getSensitiveDetectorName(), false)) {
		G4SDManager::GetSDMpointer()->Activate(getSensitiveDetectorName(), false);
	}
	// Fetch digitizer modules and set pointer to the SiPM properly to NULL.
	// This also prevents the digitizers to take action.
	G4SipmDigitizer* digitizer = reinterpret_cast<G4SipmDigitizer*>(G4DigiManager::GetDMpointer()->FindDigitizerModule(
			getDigitizerName()));
	if (digitizer) {
		digitizer->setSipm(NULL);
	}
	G4SipmVoltageTraceDigitizer* vtDigitizer =
			reinterpret_cast<G4SipmVoltageTraceDigitizer*>(G4DigiManager::GetDMpointer()->FindDigitizerModule(
					getVoltageTraceDigitizerName()));
	if (vtDigitizer) {
		vtDigitizer->setSipm(NULL);
	}
}

G4LogicalVolume* G4Sipm::build() {
	// Build SiPM solid and logical volume.
	G4VSolid * solid = new G4Box(getName() + "Solid", model->getPitch() / 2., model->getPitch() / 2.,
			model->getThickness() / 2.);
	G4SipmSensitiveDetector* sd = (G4SipmSensitiveDetector*) G4SDManager::GetSDMpointer()->FindSensitiveDetector(
			getSensitiveDetectorName(), false);
	if (!sd) {
		sd = new G4SipmSensitiveDetector(this);
	}
	G4LogicalVolume * logicalVolume = new G4LogicalVolume(solid, model->getMaterial(), getName() + "Lv", NULL, sd);
	logicalVolume->SetVisAttributes(new G4VisAttributes(G4Color::Green()));
	// Create digitizer modules.
	if (!G4DigiManager::GetDMpointer()->FindDigitizerModule(getDigitizerName())) {
		new G4SipmDigitizer(this);
	}
	if (!G4DigiManager::GetDMpointer()->FindDigitizerModule(getVoltageTraceDigitizerName())) {
		new G4SipmVoltageTraceDigitizer(this);
	}
	//
	return logicalVolume;
}

G4SipmId G4Sipm::getId() const {
	return id;
}

G4SipmModel* G4Sipm::getModel() const {
	return model;
}

G4String G4Sipm::getSensitiveDetectorName() const {
	std::ostringstream o;
	o << "g4sipmSd-" << id;
	return o.str();
}

G4String G4Sipm::getHitCollectionName() const {
	std::ostringstream o;
	o << "g4sipmHits-" << id;
	return o.str();
}

G4String G4Sipm::getDigiCollectionName() const {
	std::ostringstream o;
	o << "g4sipmDigis-" << id;
	return o.str();
}

G4String G4Sipm::getVoltageTraceDigiCollectionName() const {
	std::ostringstream o;
	o << "g4sipmVoltageTraceDigis-" << id;
	return o.str();
}

G4String G4Sipm::getDigitizerName() const {
	std::ostringstream o;
	o << "g4sipmDigitizer-" << id;
	return o.str();
}

G4String G4Sipm::getVoltageTraceDigitizerName() const {
	std::ostringstream o;
	o << "g4sipmVoltageTraceDigitizer-" << id;
	return o.str();
}
