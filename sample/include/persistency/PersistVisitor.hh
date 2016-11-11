/*
 * PersistVisitor.hh
 *
 * @date Jul 8, 2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef PERSISTENCY_PERSISTVISITOR_HH_
#define PERSISTENCY_PERSISTVISITOR_HH_

#include <boost/variant.hpp>

#include "hit/G4SipmHit.hh"
#include "digi/G4SipmDigi.hh"
#include "digi/G4SipmVoltageTraceDigi.hh"
#include "model/G4SipmModel.hh"
#include "model/G4SipmVoltageTraceModel.hh"
#include "G4SipmUiMessenger.hh"
#include "ParticleSourceMessenger.hh"

class PersistVisitor: public boost::static_visitor<> {
public:
	PersistVisitor();
	virtual ~PersistVisitor();

	virtual void open(std::string filename) = 0;
	virtual void operator ()(G4SipmHitsCollection* hc) = 0;
	virtual void operator ()(G4SipmDigiCollection* dc) = 0;
	virtual void operator ()(G4SipmVoltageTraceDigiCollection* dc) = 0;

	virtual void operator ()(G4SipmUiMessenger* m);
	virtual void operator ()(ParticleSourceMessenger* m);
	virtual void operator ()(G4SipmModel* m);
	virtual void operator ()(G4SipmVoltageTraceModel* m);

	virtual void close() = 0;
};

#endif /* PERSISTENCY_PERSISTVISITOR_HH_ */
