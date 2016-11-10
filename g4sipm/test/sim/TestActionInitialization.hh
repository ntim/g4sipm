/*
 * TestActionInitialization.hh
 *
 *  Created on: Feb 7, 2014
 *      Author: Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 */

#ifndef TESTACTIONINITIALIZATION_HH_
#define TESTACTIONINITIALIZATION_HH_

#include <G4VUserActionInitialization.hh>

#include "TestPrimaryGeneratorAction.hh"
#include "TestEventAction.hh"

class TestActionInitialization: public G4VUserActionInitialization {
private:
	G4SipmModel* model;

public:
	TestActionInitialization(G4SipmModel* _model) :
			G4VUserActionInitialization(), model(_model) {
		//
	}

	virtual ~TestActionInitialization() {
		//
	}

	virtual void Build() const {
		// Create primary generator.
		SetUserAction(new TestPrimaryGeneratorAction(model));
		// Create event action.
		SetUserAction(new TestEventAction);
	}
};

#endif /* TESTACTIONINITIALIZATION_HH_ */
