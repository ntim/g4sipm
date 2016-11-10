/*
 * G4SipmDigitizer.hh
 *
 * @created Apr 11, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMDIGITIZER_HH_
#define G4SIPMDIGITIZER_HH_

#include <G4VDigitizerModule.hh>

#include "G4Sipm.hh"
#include "digi/G4SipmDigi.hh"
#include "digi/G4SipmDigiQueue.hh"
#include "hit/G4SipmHit.hh"

/**
 * Digitizer module which creates cell triggers (G4SipmDigi) from hits (G4SipmHit).
 */
class G4SipmDigitizer: public G4VDigitizerModule {
	/**
	 * G4Sipm is a friend class to allow a proper set to NULL when the SiPM instance is destroyed.
	 */
	friend class G4Sipm;
private:
	G4Sipm* sipm;

	/**
	 * Corrects the probability of correlated noise according to the gain (weight) of the cell.
	 *
	 * @param d - the digi.
	 * @param prob - the probability.
	 * @return double - the corrected probability.
	 */
	double getRecoveryCorrectedProbability(G4SipmDigi* const d, double prob) const;
	/**
	 * Retreives the hits from the SiPM hit collection.
	 *
	 * @param queue - the digi queue to fill (outgoing).
	 * @param tMin - the minimum time found in the queue (outgoing).
	 * @param tMax - the maximum time found in the queue (outgoing).
	 */
	void addHits(G4SipmDigiQueue* queue, double* tMin, double* tMax);
	/**
	 * Adds thermal noise the the queue.
	 *
	 * @param queue - the queue to fill (outgoing).
	 * @param tMin - the minimum time of the queue.
	 * @param tMax - the maximum time of the queue.
	 */
	void addThermalNoise(G4SipmDigiQueue* queue, const double tMin, const double tMax);
	/**
	 * Adds optical crosstalk.
	 *
	 * @param d - the source trigger digi (outgoing).
	 * @param queue - the queue to fill.
	 */
	void addCrosstalk(G4SipmDigi* const d, G4SipmDigiQueue* queue);
	/**
	 * Adds afterpulses.
	 *
	 * @param d - the source trigger digi.
	 * @param queue - the queue to fill (outgoing).
	 */
	void addAfterpulses(G4SipmDigi* const d, G4SipmDigiQueue* queue);
	/**
	 * Adds a single afterpulse component.
	 *
	 * @param d - the source trigger digi.
	 * @param queue - the queue to fill (outgoing).
	 * @param probability - the probability of the afterpulse component.
	 * @param tau - the time constant of the probability.
	 */
	void addAfterpulses(G4SipmDigi* const d, G4SipmDigiQueue* queue, const double probability, const double tau);

protected:
	/**
	 * @param sipm - the SiPM to be set.
	 */
	void setSipm(G4Sipm* sipm);

public:
	/**
	 * Constructor.
	 *
	 * @param sipm - the SiPM.
	 */
	G4SipmDigitizer(G4Sipm* sipm);
	virtual ~G4SipmDigitizer();

	virtual void Digitize();
	/**
	 * @return G4SipmHitsCollection - the hit collection.
	 */
	const G4SipmHitsCollection* getHitCollection();
	/**
	 * @return G4Sipm - the SiPM to which the digitizer is attached to.
	 */
	G4Sipm* getSipm() const;
};

#endif /* G4SIPMDIGITIZER_HH_ */
