/*
 * EventAction.hh
 *
 * @date Feb 22, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef EVENTACTION_HH_
#define EVENTACTION_HH_

#include <G4UserEventAction.hh>
#include <G4HCofThisEvent.hh>
#include <G4DCofThisEvent.hh>
#include <G4VHitsCollection.hh>
#include <G4VDigiCollection.hh>
#include <G4Event.hh>

/**
 * User event action implementation. Invokes all registered digitizer modules at the end of an event.
 * Afterwards, it invokes the persistence of hits and digis via the AsciiPersistencyHandler class.
 */
class EventAction: public G4UserEventAction {
public:
	/**
	 * Constructor.
	 */
	EventAction();
	virtual ~EventAction();

	virtual void EndOfEventAction(const G4Event* event);
};

#endif /* EVENTACTION_HH_ */
