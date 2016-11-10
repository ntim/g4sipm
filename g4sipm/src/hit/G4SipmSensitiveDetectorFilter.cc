/*
 * G4SipmSensitiveDetectorFilter.cc
 *
 * @date Feb 10, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "hit/G4SipmSensitiveDetectorFilter.hh"

#include <G4Step.hh>
#include <G4StepPoint.hh>
#include <Randomize.hh>
#include <G4OpticalPhoton.hh>
#include <G4NavigationHistory.hh>

#include "G4SipmUiMessenger.hh"
#include "G4Sipm.hh"
#include "MaterialFactory.hh"
#include "FresnelUtil.hh"

G4SipmSensitiveDetectorFilter::G4SipmSensitiveDetectorFilter(const G4Sipm* _sipm) :
		G4VSDFilter("g4sipmSdFilter"), sipm(_sipm) {
	// Cache rindex properties table for speed up.
	airRindex = MaterialFactory::getInstance()->getAir()->GetMaterialPropertiesTable()->GetProperty("RINDEX");
	windowRindex = sipm->getModel()->getWindowMaterial()->GetMaterialPropertiesTable()->GetProperty("RINDEX");
	sipmRindex = sipm->getModel()->getMaterial()->GetMaterialPropertiesTable()->GetProperty("RINDEX");
}

G4SipmSensitiveDetectorFilter::~G4SipmSensitiveDetectorFilter() {
	//
}

bool G4SipmSensitiveDetectorFilter::acceptPde(double eKin, double theta) const {
	// Derive wave length and angle.
	double wavelength = CLHEP::c_light * CLHEP::h_Planck / eKin;
	double pde = sipm->getModel()->getPhotonDetectionEfficiency(wavelength);
	// Compensate fill factor.
	pde /= sipm->getModel()->getFillFactor();
	// Compensate for the reflectivity of window.
	double n1 = airRindex->Value(eKin);
	double n2 = windowRindex->Value(eKin);
	double n3 = sipmRindex->Value(eKin);
	// TODO: better handling of polarization.
	if (theta == 0.) {
		// Jackson's convetion: 
		// Perpendicular incidence only consider parallel component.
		pde /= FresnelUtil::tP(n1, n2, n3, 0.);
	} else {
		pde /= FresnelUtil::t(n1, n2, n3, theta);
	}
	// Decide on acceptance.
	if (G4UniformRand() <= pde) {
		return true;
	}
	return false;
}

bool G4SipmSensitiveDetectorFilter::acceptGeometry(double x, double y) const {
	return sipm->getModel()->isValidCellId(sipm->getModel()->getCellId(x, y, true));
}

G4bool G4SipmSensitiveDetectorFilter::Accept(const G4Step* step) const {
	G4double eKin = step->GetPreStepPoint()->GetKineticEnergy();
	G4ThreeVector worldMom = step->GetPreStepPoint()->GetMomentumDirection();
	G4ThreeVector localMom = step->GetPreStepPoint()->GetTouchable()->GetHistory()->GetTopTransform().TransformAxis(
			worldMom);
	G4ThreeVector worldPos = step->GetPreStepPoint()->GetPosition();
	G4ThreeVector localPos = step->GetPreStepPoint()->GetTouchable()->GetHistory()->GetTopTransform().TransformPoint(
			worldPos);
	// Angle of incidence.
	double theta = acos(fabs(localMom.z() / localMom.mag()));
	// Check for particle type.
	if (step->GetTrack()->GetParticleDefinition() != G4OpticalPhoton::Definition()) {
		return false;
	}
	// Check if the particle has been reflected.
	if (step->GetPreStepPoint()->GetMaterial() == sipm->getModel()->getMaterial()
			&& step->GetPostStepPoint()->GetMaterial() == sipm->getModel()->getWindowMaterial()) {
		return false;
	}
	// Check geometry.
	if (G4SipmUiMessenger::getInstance()->isFilterGeometry() && !acceptGeometry(localPos.x(), localPos.y())) {
		return false;
	}
	// Check photon detection efficiency.
	if (G4SipmUiMessenger::getInstance()->isFilterPde() && !acceptPde(eKin, theta)) {
		return false;
	}
	// Kill photon because it can not pass through the detector.
	step->GetTrack()->SetTrackStatus(fStopAndKill);
	// Everything ok, return true.
	return true;
}
