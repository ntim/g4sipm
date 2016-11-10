/*
 * OpticalPhysicsList.hh
 *
 * @date Sep 12, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef OPTICALPHYSICSLIST_HH_
#define OPTICALPHYSICSLIST_HH_

#include <G4VModularPhysicsList.hh>
#include <G4EmStandardPhysics.hh>
#include <G4OpticalPhysics.hh>

/**
 * Physics list registering all optical processes described in G4OpticalPhysics.
 */
class OpticalPhysicsList: public G4VModularPhysicsList {
public:
	/**
	 * Constructor.
	 *
	 * @param verbose - the verbosity of the physics process (0).
	 */
	OpticalPhysicsList(int verbose = 0, std::vector<G4OpticalProcessIndex> deactivate = std::vector<G4OpticalProcessIndex>());
	virtual ~OpticalPhysicsList();

	virtual void ConstructParticle();
	virtual void SetCuts();
};

#endif /* OPTICALPHYSICSLIST_HH_ */
