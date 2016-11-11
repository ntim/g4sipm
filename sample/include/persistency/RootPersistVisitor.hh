/*
 * RootPersistVisitor.hh
 *
 * @date Feb 12, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef ROOTPERSISTVISITOR_HH_
#define ROOTPERSISTVISITOR_HH_

#include "persistency/PersistencyHandler.hh"

#include <CLHEP/Vector/ThreeVector.h>

class TFile;

class RootPersistVisitor: public PersistVisitor {
private:
	TFile* file;

protected:
	void copy(CLHEP::Hep3Vector v, double* a);
	TFile* getFile() const;

public:
	virtual void open(std::string filename);
	virtual void operator ()(G4SipmHitsCollection* hc);
	virtual void operator ()(G4SipmDigiCollection* dc);
	virtual void operator ()(G4SipmVoltageTraceDigiCollection* dc);
	virtual void operator ()(G4SipmUiMessenger* m);
	virtual void operator ()(ParticleSourceMessenger* m);
	virtual void operator ()(G4SipmModel* m);
	virtual void operator ()(G4SipmVoltageTraceModel* m);
	virtual void close();
};

#endif /* ROOTPERSISTVISITOR_HH_ */
