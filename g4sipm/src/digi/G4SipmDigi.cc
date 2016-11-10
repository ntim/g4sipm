/*
 * Digi.cc
 *
 * @created Apr 11, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "digi/G4SipmDigi.hh"

G4ThreadLocal G4Allocator<G4SipmDigi>* G4SipmDigiAllocator = 0;

G4SipmDigi::G4SipmDigi() :
		sipmId(0), time(0), cellId(0), weight(1), type(UNDEFINED) {
	//
}

G4SipmDigi::~G4SipmDigi() {
}

G4SipmDigi::G4SipmDigi(const G4SipmDigi& right) {
	sipmId = right.getSipmId();
	time = right.getTime();
	cellId = right.getCellId();
	weight = right.getWeight();
	type = right.getType();
}

const G4SipmDigi& G4SipmDigi::operator=(const G4SipmDigi& right) {
	sipmId = right.sipmId;
	time = right.time;
	type = right.type;
	weight = right.weight;
	return *this;
}

int G4SipmDigi::operator==(const G4SipmDigi& right) const {
	return (sipmId == right.sipmId) && (time == right.time) && (type == right.type) && (weight == right.weight);
}

G4SipmId G4SipmDigi::getSipmId() const {
	return sipmId;
}

void G4SipmDigi::setSipmId(G4SipmId _sipmId) {
	sipmId = _sipmId;
}

G4SipmCellId G4SipmDigi::getCellId() const {
	return cellId;
}

void G4SipmDigi::setCellId(G4SipmCellId _cellId) {
	cellId = _cellId;
}

double G4SipmDigi::getTime() const {
	return time;
}

void G4SipmDigi::setTime(double _time) {
	time = _time;
}

G4SipmDigiType G4SipmDigi::getType() const {
	return type;
}

void G4SipmDigi::setType(G4SipmDigiType _type) {
	type = _type;
}

double G4SipmDigi::getWeight() const {
	return weight;
}

void G4SipmDigi::setWeight(double _weight) {
	weight = _weight;
}
