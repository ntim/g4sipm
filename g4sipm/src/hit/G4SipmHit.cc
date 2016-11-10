/*
 * G4SipmHit.cc
 *
 * @date Mar 13, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "hit/G4SipmHit.hh"

#include <G4Point3D.hh>
#include <G4Circle.hh>
#include <G4VisAttributes.hh>
#include <G4Color.hh>
#include <G4ParticleTable.hh>
#include <G4VVisManager.hh>

G4ThreadLocal G4Allocator<G4SipmHit>* G4SipmHitAllocator = 0;

G4SipmHit::G4SipmHit() :
		G4VHit(), sipmId(-1), pdgId(-1), trackId(-1), eKin(0), time(0), weight(0) {
	//
}

G4SipmHit::~G4SipmHit() {
	//
}

void G4SipmHit::Draw() {
	G4VVisManager* visManager = G4VVisManager::GetConcreteInstance();
	if (visManager != NULL) {
		G4Point3D point(worldPosition.x(), worldPosition.y(), worldPosition.z());
		const HepGeom::Transform3D transform;
		G4Circle circle(point);
		circle.SetScreenSize(2.);
		circle.SetFillStyle(G4VMarker::filled);
		circle.SetVisAttributes(new G4VisAttributes(G4Color::Green()));
		G4VVisManager::GetConcreteInstance()->Draw(circle, transform);
	}
}

double G4SipmHit::getEKin() const {
	return eKin;
}

void G4SipmHit::setEKin(double _eKin) {
	eKin = _eKin;
}

const CLHEP::Hep3Vector& G4SipmHit::getMomentum() const {
	return momentum;
}

void G4SipmHit::setMomentum(const CLHEP::Hep3Vector& _momentum) {
	momentum = _momentum;
}

int G4SipmHit::getPdgId() const {
	return pdgId;
}

void G4SipmHit::setPdgId(int _pdgId) {
	pdgId = _pdgId;
}

int G4SipmHit::getTrackId() const {
	return trackId;
}

void G4SipmHit::setTrackId(int _trackId) {
	trackId = _trackId;
}

const CLHEP::Hep3Vector& G4SipmHit::getPosition() const {
	return position;
}

void G4SipmHit::setPosition(const CLHEP::Hep3Vector& _position) {
	position = _position;
}

G4SipmId G4SipmHit::getSipmId() const {
	return sipmId;
}

void G4SipmHit::setSipmId(G4SipmId _sipmId) {
	sipmId = _sipmId;
}

const CLHEP::Hep3Vector& G4SipmHit::getStartMomentum() const {
	return startMomentum;
}

void G4SipmHit::setStartMomentum(const CLHEP::Hep3Vector& _startMomentum) {
	startMomentum = _startMomentum;
}

const CLHEP::Hep3Vector& G4SipmHit::getStartPosition() const {
	return startPosition;
}

void G4SipmHit::setStartPosition(const CLHEP::Hep3Vector& _startPosition) {
	startPosition = _startPosition;
}

double G4SipmHit::getTime() const {
	return time;
}

void G4SipmHit::setTime(double _time) {
	time = _time;
}

double G4SipmHit::getWeight() const {
	return weight;
}

void G4SipmHit::setWeight(double _weight) {
	weight = _weight;
}

const CLHEP::Hep3Vector& G4SipmHit::getWorldPosition() const {
	return worldPosition;
}

void G4SipmHit::setWorldPosition(const CLHEP::Hep3Vector& _worldPosition) {
	worldPosition = _worldPosition;
}

G4ParticleDefinition* G4SipmHit::getParticleDefinition() const {
	return G4ParticleTable::GetParticleTable()->FindParticle(pdgId);
}
