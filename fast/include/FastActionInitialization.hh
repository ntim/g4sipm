/*
 * FastActionInitialization.hh
 *
 *  Created on: Jun 3, 2015
 *      Author: niggemann
 */

#ifndef FASTACTIONINITIALIZATION_HH_
#define FASTACTIONINITIALIZATION_HH_

#include <ActionInitialization.hh>

#include "FastEventAction.hh"
#include "FastParticleSource.hh"

class FastActionInitialization: public ActionInitialization {
public:
	FastActionInitialization(std::string path): ActionInitialization(path) {
		//
	}

	virtual void Build() const {
		ActionInitialization::Build();
		// Overwrite.
		SetUserAction(new FastEventAction);
		SetUserAction(new FastParticleSource);
	}
};

#endif /* FASTACTIONINITIALIZATION_HH_ */
