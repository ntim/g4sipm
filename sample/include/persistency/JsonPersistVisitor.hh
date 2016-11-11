/*
 * JsonPersistVisitor.hh
 *
 * @date Feb 11, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef JSONPERSISTVISITOR_HH_
#define JSONPERSISTVISITOR_HH_

#include "persistency/PersistencyHandler.hh"

#include <fstream>

class json_t;

class JsonPersistVisitor: public PersistVisitor {
private:
	std::ofstream out;

	void write(json_t* j);
	json_t* json_vector(const CLHEP::Hep3Vector& v) const;

protected:
	void operator ()(G4SipmHit* hit, G4SipmHitsCollection* hc);
	void operator ()(G4SipmDigi* digi, G4SipmDigiCollection* dc);
	void operator ()(G4SipmVoltageTraceDigi* digi, G4SipmVoltageTraceDigiCollection* dc);

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

#endif /* JSONPERSISTVISITOR_HH_ */
