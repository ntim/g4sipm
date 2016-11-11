/*
 * ActionInitialization.hh
 *
 * @date Feb 10, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef ACTIONINITIALIZATION_HH_
#define ACTIONINITIALIZATION_HH_

#include <string>

#include <G4VUserActionInitialization.hh>

class ActionInitialization: public G4VUserActionInitialization {
private:
	std::string filename;

public:
	ActionInitialization(std::string path);
	virtual ~ActionInitialization();

	virtual void Build() const;
};

#endif /* ACTIONINITIALIZATION_HH_ */
