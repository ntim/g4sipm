/*
 * G4SipmSensitiveDetector.hh
 *
 * @date Jan 11, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMSENSITIVEDETECTOR_HH_
#define G4SIPMSENSITIVEDETECTOR_HH_

#include <G4VSensitiveDetector.hh>
#include <G4SDManager.hh>
#include <G4Timer.hh>

#include "G4Sipm.hh"
#include "hit/G4SipmSensitiveDetectorFilter.hh"
#include "hit/G4SipmHit.hh"

/**
 * This sensitive detector handles the sensitive parts of a G4Sipm and creates G4SipmHit.
 * Registers itself to the G4SDManager. Makes use of the G4SipmSensitiveDetectorFilter.
 */
class G4SipmSensitiveDetector: public G4VSensitiveDetector {
private:
	const G4Sipm* sipm;
	G4SipmHitsCollection* hitsCollection;
	G4Timer timer;

protected:
	/**
	 * Creates a G4SipmHit object based on the information stored in the step.
	 *
	 * @param step - the step.
	 * @return G4SipmHit* - a hit.
	 */
	virtual G4SipmHit* createHit(G4Step* step);

public:

	/**
	 * Constructor.
	 *
	 * @param sipm - the SiPM to be attached to.
	 */
	G4SipmSensitiveDetector(const G4Sipm* sipm);
	virtual ~G4SipmSensitiveDetector();

	virtual void Initialize(G4HCofThisEvent*);
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	virtual void EndOfEvent(G4HCofThisEvent*);
};

#endif /* G4SIPMSENSITIVEDETECTOR_HH_ */
