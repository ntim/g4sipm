/*
 * PersistVisitor.cc
 *
 * @date Jul 8, 2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
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
