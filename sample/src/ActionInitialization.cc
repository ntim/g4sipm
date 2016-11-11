/*
 * ActionInitialization.cc
 *
 * @date Feb 10, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <ActionInitialization.hh>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include <G4RunManager.hh>
#include <G4GeometryTolerance.hh>

#include "G4SipmUiMessenger.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "ParticleSource.hh"
#include "persistency/PersistencyHandlerMessenger.hh"

ActionInitialization::ActionInitialization(std::string path) :
		G4VUserActionInitialization() {
	if (boost::filesystem::is_directory(path)) {
		std::string time = boost::posix_time::to_iso_string(boost::posix_time::microsec_clock::local_time());
		filename = boost::str(boost::format("%s/g4sipm_%s") % path % time);
	} else {
		filename = path;
	}
}

ActionInitialization::~ActionInitialization() {
	//
}

void ActionInitialization::Build() const {
	// Load messengers.
	G4SipmUiMessenger::getInstance();
	ParticleSourceMessenger::getInstance();
	PersistencyHandlerMessenger::getInstance();
	// Set user actions.
	SetUserAction(new RunAction(filename));
	SetUserAction(new EventAction);
	SetUserAction(new ParticleSource);
	// Retreive detector construction.
	const DetectorConstruction* detector =
			(const DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	// Set plane type and position to match SiPM.
	ParticleSourceMessenger::getInstance()->setA(detector->getSipmModel()->getPitch());
	ParticleSourceMessenger::getInstance()->setB(detector->getSipmModel()->getPitch());
	const double surfaceTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
	ParticleSourceMessenger::getInstance()->setPos(G4ThreeVector(0, 0, 10. * surfaceTolerance));
}
