/*
 * G4SipmDetectorFilterMessenger.cc
 *
 * @date Sep 13, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "G4SipmUiMessenger.hh"
#include "G4UiMessengerUtil.hh"

#include <boost/date_time.hpp>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Random/Random.h>
#include <CLHEP/Random/MTwistEngine.h>

const std::string G4SipmUiMessenger::MACRO_FILE_NAME = "g4sipm.mac";

G4SipmUiMessenger::G4SipmUiMessenger() :
		G4UImessenger() {
	setDefaults();
	// Create new directories.
	new G4UIdirectory("/g4sipm/");
	new G4UIdirectory("/g4sipm/filter/");
	new G4UIdirectory("/g4sipm/noise/");
	// Initialize commands.
	verboseCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/", "verbose", verbose);
	seedCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/", "seed", seed);
	filterGeometryCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/filter/", "geometry", filterGeometry);
	filterPdeCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/filter/", "pde", filterPde);
	filterTimingCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/filter/", "timing", filterTiming);
	noiseThermalCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/noise/", "thermal", noiseThermal);
	noisePreThermalCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/noise/", "preThermal", noisePreThermal);
	noisePostThermalCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/noise/", "postThermal", noisePostThermal);
	noiseAfterpulseCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/noise/", "afterpulse", noiseAfterpulse);
	noiseCrosstalkCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/noise/", "crosstalk", noiseCrosstalk);
	noiseCrosstalkNeighboursCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/noise/", "crosstalkNeighbours",
			noiseCrosstalkNeighbours);
	noiseIfNoSignalCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/noise/", "ifNoSignal", noiseIfNoSignal);
	digitizeHitsCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/digitize/", "hits", digitizeHits);
	digitizeTraceCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/digitize/", "trace", digitizeTrace);
	simulateShuntresistorCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/digitize/", "shuntresistor",
			simulateShuntresistor);
	shuntresistorRecoveryTimeCmd = G4UiMessengerUtil::createCmd(this, "/g4sipm/digitize/", "shuntresistorRecoveryTime",
			shuntresistorRecoveryTime, "ns");
	// Try to load defaults from the macro file.
	G4UiMessengerUtil::executeMacro(MACRO_FILE_NAME, true);
}

G4SipmUiMessenger::~G4SipmUiMessenger() {
	delete verboseCmd;
	delete seedCmd;
	delete filterGeometryCmd;
	delete filterPdeCmd;
	delete filterTimingCmd;
	delete noiseThermalCmd;
	delete noisePreThermalCmd;
	delete noisePostThermalCmd;
	delete noiseAfterpulseCmd;
	delete noiseCrosstalkCmd;
	delete noiseCrosstalkNeighboursCmd;
	delete noiseIfNoSignalCmd;
	delete digitizeHitsCmd;
	delete digitizeTraceCmd;
	delete simulateShuntresistorCmd;
	delete shuntresistorRecoveryTimeCmd;
}

G4SipmUiMessenger* G4SipmUiMessenger::getInstance() {
	static G4SipmUiMessenger* instance = new G4SipmUiMessenger;
	return instance;
}

inline long now() {
	const boost::posix_time::ptime time_t_epoch(boost::gregorian::date(1970, 1, 1));
	const boost::posix_time::ptime ptime = boost::posix_time::microsec_clock::local_time();
	return (ptime - time_t_epoch).total_microseconds();
}

void G4SipmUiMessenger::setDefaults() {
	verbose = 0;
	seed = now();
	filterGeometry = true;
	filterPde = true;
	filterTiming = true;
	noiseThermal = true;
	noisePreThermal = 3;
	noisePostThermal = 1;
	noiseAfterpulse = true;
	noiseCrosstalk = true;
	noiseCrosstalkNeighbours = 4;
	noiseIfNoSignal = true;
	digitizeHits = true;
	digitizeTrace = true;
	simulateShuntresistor = false;
	shuntresistorRecoveryTime = 30. * CLHEP::nanosecond;
	initRandomEngine();
}

void G4SipmUiMessenger::initRandomEngine() {
	std::cout << "G4SipmUiMessenger::initRandomEngine(): CLHEP::MTwistEngine with seed " << getSeed() << std::endl;
	CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine(getSeed()));
}

void G4SipmUiMessenger::SetNewValue(G4UIcommand* cmd, G4String value) {
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, verboseCmd, value, &verbose);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, seedCmd, value, &seed);
	// Update random number generator with the new seed.
	if (cmd == seedCmd) {
		initRandomEngine();
	}
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, filterGeometryCmd, value, &filterGeometry);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, filterPdeCmd, value, &filterPde);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, filterTimingCmd, value, &filterTiming);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, noiseThermalCmd, value, &noiseThermal);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, noisePreThermalCmd, value, &noisePreThermal);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, noisePostThermalCmd, value, &noisePostThermal);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, noiseAfterpulseCmd, value, &noiseAfterpulse);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, noiseCrosstalkCmd, value, &noiseCrosstalk);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, noiseCrosstalkNeighboursCmd, value, &noiseCrosstalkNeighbours);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, noiseIfNoSignalCmd, value, &noiseIfNoSignal);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, digitizeHitsCmd, value, &digitizeHits);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, digitizeTraceCmd, value, &digitizeTrace);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, simulateShuntresistorCmd, value, &simulateShuntresistor);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, shuntresistorRecoveryTimeCmd, value, &shuntresistorRecoveryTime);
}

