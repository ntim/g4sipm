/*
 * RunAction.hh
 *
 * @date 26.02.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef RUNACTION_HH_
#define RUNACTION_HH_

#include <string>
#include <G4UserRunAction.hh>
#include <G4Timer.hh>

class PersistencyHandler;

/**
 * User run action implementation. Initializes the output file at the beginning of each run
 * and closes it afterwards. It also measures the execution time.
 */
class RunAction: public G4UserRunAction {
private:
	G4Timer timer;
	std::string filename;
	PersistencyHandler* persistencyHandler;

public:
	/**
	 * Constructor.
	 *
	 * @param filename - the filename of the output file.
	 */
	RunAction(std::string filename);
	virtual ~RunAction();

	virtual void BeginOfRunAction(const G4Run* run);
	virtual void EndOfRunAction(const G4Run* run);

	/**
	 * @return string - the filename.
	 */
	std::string getFilename() const;
	/**
	 * @return - the persistency handler.
	 */
	PersistencyHandler* getPersistencyHandler() const;
};

#endif /* RUNACTION_HH_ */
