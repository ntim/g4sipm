/*
 * AsciiPersistencyHandlerMessenger.hh
 *
 * @date 24.08.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef ASCIIPERSISTENCYHANDLERMESSENGER_HH_
#define ASCIIPERSISTENCYHANDLERMESSENGER_HH_

#include <G4UImessenger.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithABool.hh>

/**
 * Ui messenger controlling the persistence files. The options tMin and tMax will be initialized to the
 * settings of the general particle source plus/minus 1000 nanoseconds.
 *
 * Commands:
 * /g4sipm/persist/tMin
 * /g4sipm/persist/tMax
 * /g4sipm/persist/hits
 * /g4sipm/persist/digis
 */
class PersistencyHandlerMessenger: public G4UImessenger {
private:
	double tMin;
	double tMax;
	bool persistHits;
	bool persistDigis;

	G4UIcmdWithADoubleAndUnit* tMinCmd;
	G4UIcmdWithADoubleAndUnit* tMaxCmd;
	G4UIcmdWithABool* persistHitsCmd;
	G4UIcmdWithABool* persistDigisCmd;

	PersistencyHandlerMessenger();
public:
	/**
	 * @return AsciiPersistencyHandlerMessenger - the singleton.
	 */
	static PersistencyHandlerMessenger* getInstance();

	virtual ~PersistencyHandlerMessenger();

	virtual void SetNewValue(G4UIcommand * command, G4String newValue);

	/**
	 * @return double - the maximum time stamp to persist hits and digis for.
	 */
	double getTMax() const;
	/**
	 * @return double - the minimum time stamp to persist hits and digis for.
	 */
	double getTMin() const;
	/**
	 * @return bool - true if to persist the hits.
	 */
	bool isPersistHits() const;
	/**
	 * @return bool - true if to persist the digis.
	 */
	bool isPersistDigis() const;
};

#endif /* ASCIIPERSISTENCYHANDLERMESSENGER_HH_ */
