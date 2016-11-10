/*
 * DigiQueue.cc
 *
 * @created Apr 11, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "digi/G4SipmDigiQueue.hh"

G4SipmDigiQueue::G4SipmDigiQueue() {
	//
}

G4SipmDigiQueue::~G4SipmDigiQueue() {
	//
}

void G4SipmDigiQueue::push(G4SipmDigi* digi) {
	if (digi != NULL) {
		G4SipmAbstractDigiQueue::push(digi);
	}
}

void G4SipmDigiQueue::pushAll(const std::vector<G4SipmDigi*>& newDigis) {
	if (newDigis.size() != 0) {
		for (std::vector<G4SipmDigi*>::const_iterator it = newDigis.begin(); it != newDigis.end(); it++) {
			push(*it);
		}
	}
}

G4SipmDigi* G4SipmDigiQueue::next() {
	if (hasNext()) {
		G4SipmDigi* digi = top();
		pop();
		return digi;
	}
	return NULL;
}

bool G4SipmDigiQueue::hasNext() const {
	return size() > 0;
}
