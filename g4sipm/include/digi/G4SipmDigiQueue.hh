/*
 * G4SipmDigiQueue.hh
 *
 * @created Apr 11, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMDIGIQUEUE_HH_
#define G4SIPMDIGIQUEUE_HH_

#include <vector>
#include <queue>

#include "digi/G4SipmDigi.hh"

/**
 * Comparator for two G4SipmDigi instances. Evaluates the trigger times.
 */
struct G4SipmDigiComparatorReverse: public std::binary_function<G4SipmDigi*, G4SipmDigi*, bool> {
	bool operator()(G4SipmDigi*& left, G4SipmDigi*& right) const {
		return left->getTime() > right->getTime();
	}
};

typedef std::priority_queue<G4SipmDigi*, std::vector<G4SipmDigi*>, G4SipmDigiComparatorReverse> G4SipmAbstractDigiQueue;

/**
 * Container which automatically sorts G4SipmDigi according to the time and acts like a queue.
 */
class G4SipmDigiQueue: public G4SipmAbstractDigiQueue {
public:
	/**
	 * Constructor.
	 */
	G4SipmDigiQueue();
	virtual ~G4SipmDigiQueue();

	/**
	 * @param digi - the digi to add.
	 */
	virtual void push(G4SipmDigi* digi);
	/**
	 * @param digis - the digis to add.
	 */
	void pushAll(const std::vector<G4SipmDigi*>& digis);
	/**
	 * @return G4SipmDigi - returns the next element of the queue and removes it.
	 */
	G4SipmDigi* next();
	/**
	 * @return bool - true if there is at least one element in the queue.
	 */
	bool hasNext() const;
};

#endif /* G4SIPMDIGIQUEUE_HH_ */
