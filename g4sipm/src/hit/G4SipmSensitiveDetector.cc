/*
 * G4SipmSensitiveDetector.cc
 *
 * @date Jan 11, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "hit/G4SipmSensitiveDetector.hh"

#include <G4SDManager.hh>

#include "G4Sipm.hh"
#include "G4SipmUiMessenger.hh"

G4SipmSensitiveDetector::G4SipmSensitiveDetector(const G4Sipm* _sipm) :
		G4VSensitiveDetector(_sipm->getSensitiveDetectorName()), sipm(_sipm), hitsCollection(NULL) {
	// Register name to the inherited collection.
	collectionName.push_back(sipm->getHitCollectionName());
	// Register to Geant4.
	G4SDManager::GetSDMpointer()->AddNewDetector(this);
	// Sets the filter.
	SetFilter(new G4SipmSensitiveDetectorFilter(sipm));
}

G4SipmSensitiveDetector::~G4SipmSensitiveDetector() {
	//
}

void G4SipmSensitiveDetector::Initialize(G4HCofThisEvent*) {
	// Initialize hits collection.
	hitsCollection = new G4SipmHitsCollection(SensitiveDetectorName, collectionName[0]);
	if (G4SipmUiMessenger::getInstance()->getVerbose() > 0) {
		std::cout << "G4SipmSensitiveDetector::Initialize(): create hits collection \"" << collectionName[0]
				<< "\" for sensitive detector \"" << SensitiveDetectorName << "\"." << std::endl;
	}
	timer.Start();
}

G4SipmHit* G4SipmSensitiveDetector::createHit(G4Step* step) {
	const G4DynamicParticle* particle = step->GetTrack()->GetDynamicParticle();
	G4StepPoint* stepPoint = step->GetPreStepPoint();
	const G4TouchableHistory* touchable = reinterpret_cast<const G4TouchableHistory*>(stepPoint->GetTouchable());
	// Create new hit.
	G4SipmHit* hit = new G4SipmHit();
	hit->setEKin(particle->GetKineticEnergy());
	hit->setMomentum(particle->GetMomentumDirection());
	hit->setPdgId(particle->GetPDGcode());
	hit->setTrackId(step->GetTrack()->GetTrackID());
	hit->setSipmId(sipm->getId());
	hit->setTime(stepPoint->GetGlobalTime());
	hit->setWeight(stepPoint->GetWeight());
	// Get local position.
	G4ThreeVector globalPos = stepPoint->GetPosition();
	hit->setWorldPosition(globalPos);
	G4ThreeVector localPos = touchable->GetHistory()->GetTopTransform().TransformPoint(globalPos);
	hit->setPosition(localPos);
	// Set start properties.
	hit->setStartPosition(step->GetTrack()->GetVertexPosition());
	hit->setStartMomentum(step->GetTrack()->GetVertexMomentumDirection());
	return hit;
}

G4bool G4SipmSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
	if (G4SipmUiMessenger::getInstance()->getVerbose() > 1) {
		std::cout << "G4SipmSensitiveDetector::ProcessHits(): sipm-" << sipm->getId() << " is hit." << std::endl;
	}
	// Create hit.
	G4SipmHit* hit = createHit(step);
	hitsCollection->insert(hit);
	// Stop track.
	step->GetTrack()->SetTrackStatus(fStopAndKill);
	return true;
}

void G4SipmSensitiveDetector::EndOfEvent(G4HCofThisEvent* hCof) {
	int collectionId = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	hCof->AddHitsCollection(collectionId, hitsCollection);
	timer.Stop();
	std::cout << "G4SipmSensitiveDetector::EndOfEvent(): added hits collection \"" << collectionName[0]
			<< "\" for sensitive detector \"" << SensitiveDetectorName << "\" with " << hitsCollection->GetSize()
			<< " entries (" << timer << ")." << std::endl;
}
