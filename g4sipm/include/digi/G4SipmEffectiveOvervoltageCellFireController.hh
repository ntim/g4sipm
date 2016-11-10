/*
 * G4SipmEffectiveOvervoltageCellFireController.hh
 *
 *  Created on: Feb 17, 2014
 *      Author: Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 */

#ifndef G4SIPMEFFECTIVEOVERVOLTAGECELLFIRECONTROLLER_HH_
#define G4SIPMEFFECTIVEOVERVOLTAGECELLFIRECONTROLLER_HH_

#include <digi/G4SipmCellFireController.hh>

class G4SipmEffectiveOvervoltageCellFireController: public G4SipmCellFireController {
private:
	double effectiveOvervoltage;
	double shuntResistor;
	double quenchingResistor;
	double tLast;
	unsigned int nCoincident;
	double tau;
	double vu;

public:
	G4SipmEffectiveOvervoltageCellFireController(G4SipmModel* model, double t0);

	virtual bool fire(G4SipmDigi* d);
};

#endif /* G4SIPMEFFECTIVEOVERVOLTAGECELLFIRECONTROLLER_HH_ */
