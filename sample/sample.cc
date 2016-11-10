/*
 * sample.cc
 *
 * @date Sep 12, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <boost/program_options.hpp>

#include <G4RunManager.hh>
#include <G4UImanager.hh>
#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif
#ifdef G4UI_USE
#include <G4UIExecutive.hh>
#ifdef G4UI_USE_QT
#include <G4UIQt.hh>
#endif
#endif

#include "OpticalPhysicsList.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "ProgramOptionsUtil.hh"
#include "ParticleSourceMessenger.hh"

int main(int argc, char** argv) {
	std::cout
			<< "G4SiPM SampleSim Copyright (C) 2012 Tim Tim Niggemann, III Phys. Inst. A, RWTH Aachen University et. al."
			<< std::endl;
	std::cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n"
			<< "This is free software, and you are welcome to redistribute it\n"
			<< "under certain conditions; type `show c' for details." << std::endl;
	// Define and parse the program options.
	namespace po = boost::program_options;
	po::options_description desc("Options");
	desc.add_options()("help", "Produce help message.") //
	("mac", po::value<std::string>(), "Macro input file.") //
	("output", po::value<std::string>()->default_value("./results"),
			"Output directory (automatically appends timestamp to filename) or filename (./results).") //
	("model", po::value<std::string>()->default_value("HamamatsuS1036211100"),
			"The SiPM model name or path to .properties file.") //
	("housing", po::value<std::string>()->default_value("default"),
			"The SiPM housing type ('ceramic', 'smd', 'default'.") //,
	("temperature", po::value<double>(), "override the models default temperature setting") //
	("bias-voltage", po::value<double>(), "override the models default bias voltage");
	po::variables_map vm = ProgramOptionsUtil::parse(argc, argv, desc, true);
	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 0;
	}
	// Construct the run manager.
	G4RunManager* runManager = new G4RunManager();
	// Set mandatory initialization classes.
	DetectorConstruction* detectorConstruction = new DetectorConstruction(vm["model"].as<std::string>(),
			vm["housing"].as<std::string>());
	if (vm.count("temperature")) {
		detectorConstruction->getSipmModel()->setTemperature(
				vm["temperature"].as<double>() * CLHEP::kelvin + CLHEP::STP_Temperature);
	}
	if (vm.count("bias-voltage")) {
		detectorConstruction->getSipmModel()->setBiasVoltage(vm["bias-voltage"].as<double>() * CLHEP::volt);
	}
	runManager->SetUserInitialization(detectorConstruction);
	// Deactivate all irrelevant processes.
	std::vector<G4OpticalProcessIndex> deactivate;
	deactivate.push_back(kCerenkov);
	deactivate.push_back(kScintillation);
	deactivate.push_back(kRayleigh);
	deactivate.push_back(kMieHG);
	deactivate.push_back(kWLS);
	runManager->SetUserInitialization(new OpticalPhysicsList(0,deactivate));
	runManager->SetUserInitialization(new ActionInitialization(vm["output"].as<std::string>()));
	// Initialize particle source messenger with command line arguments.
	ParticleSourceMessenger::getInstance()->parseProgramOptions(argc, argv);
	// Initialize G4 kernel.
	runManager->Initialize();
	// Create visualization manager.
	G4UImanager* uiManager = G4UImanager::GetUIpointer();
#ifdef G4VIS_USE
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif
	// Create UI manager.
#ifdef G4UI_USE
	uiManager = G4UImanager::GetUIpointer();
	// Batch mode.
	if (vm.count("mac")) {
		uiManager->ApplyCommand((G4String("/control/execute ") + vm["mac"].as<std::string>()));
	} else {
		// Interactive mode.
#ifdef G4UI_USE_QT
		// Create Qt UI.
		G4UIQt* ui = new G4UIQt(argc, argv);
		uiManager->ApplyCommand("/control/execute vis-qt.mac");
		ui->SessionStart();
		delete ui;
#else
		// Create default UI.
		G4UIExecutive * ui = new G4UIExecutive(argc, argv);
		uiManager->ApplyCommand("/control/execute vis.mac");
		ui->SessionStart();
		delete ui;
#endif
	}
#endif
	// Tear down.
	delete runManager;
	return 0;
}
