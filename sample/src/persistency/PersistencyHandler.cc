/*
 * PersistencyHandler.cc
 *
 * @date Feb 11, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "persistency/PersistencyHandler.hh"

#include <iostream>

PersistencyHandler::PersistencyHandler(PersistVisitor* _visitor): visitor(_visitor) {
	//
}

PersistencyHandler::~PersistencyHandler() {
	delete visitor;
}

void PersistencyHandler::persist(Persistable p) {
	boost::apply_visitor(*visitor, p);
}

void PersistencyHandler::open(std::string filename) {
	visitor->open(filename);
}

void PersistencyHandler::close() {
	visitor->close();
}

PersistVisitor* PersistencyHandler::getVisitor() const {
	return visitor;
}
