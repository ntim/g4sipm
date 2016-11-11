/*
 * PersistVisitorFactory.cc
 *
 * @date Jul 8, 2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "persistency/PersistVisitorFactory.hh"

#include <boost/algorithm/string.hpp>

#ifdef WITH_ROOT
#include "persistency/RootPersistVisitor.hh"
#endif
#ifdef WITH_SQLITE
#include "persistency/SqlitePersistVisitor.hh"
#endif
#include "persistency/JsonPersistVisitor.hh"

PersistVisitorFactory::~PersistVisitorFactory() {
	//
}
PersistVisitorFactory::PersistVisitorFactory() {
	//
}

PersistVisitorFactory* PersistVisitorFactory::getInstance() {
	static PersistVisitorFactory* instance = new PersistVisitorFactory;
	return instance;
}

PersistVisitor* PersistVisitorFactory::create(std::string filename) {
#ifdef WITH_ROOT
	if(boost::algorithm::ends_with(filename, "root")) {
		return new RootPersistVisitor;
	}
#endif
#ifdef WITH_SQLITE
	if(boost::algorithm::ends_with(filename, "sqlite")) {
		return new SqlitePersistVisitor;
	}
#endif
	return new JsonPersistVisitor;
}
