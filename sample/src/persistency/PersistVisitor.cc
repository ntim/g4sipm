/*
 * PersistVisitor.cc
 *
 *  Created on: Jul 8, 2015
 *      Author: niggemann
 */

#include <persistency/PersistVisitor.hh>

PersistVisitor::PersistVisitor() {
	//
}

PersistVisitor::~PersistVisitor() {
	//
}

void PersistVisitor::operator ()(ParticleSourceMessenger*) {
	std::cerr << "PersistVisitor::operator (ParticleSourceMessenger* dc): not implemented." << std::endl;
}

void PersistVisitor::operator ()(G4SipmModel*) {
	std::cerr << "PersistVisitor::operator (G4SipmModel* dc): not implemented." << std::endl;
}

void PersistVisitor::operator ()(G4SipmVoltageTraceModel*) {
	std::cerr << "PersistVisitor::operator (G4SipmVoltageTraceModel* dc): not implemented." << std::endl;
}

void PersistVisitor::operator ()(G4SipmUiMessenger*) {
	std::cerr << "PersistVisitor::operator (G4SipmVoltageTraceModel* dc): not implemented." << std::endl;
}
