/*
 * G4SipmDetectorFilterMessenger.hh
 *
 * @date Sep 13, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMDETECTORFILTERMESSENGER_HH_
#define G4SIPMDETECTORFILTERMESSENGER_HH_

#include <G4UImessenger.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

/**
 * Geant4 Ui messenger (singleton) for manipulation of the G4SiPM simulation. Can enable/disable certain steps of the
 * simulation and truncate the results.
 *
 * Commands:
 * /g4sipm/verbose
 * /g4sipm/seed
 * /g4sipm/filter/geometry
 * /g4sipm/filter/pde
 * /g4sipm/filter/timing
 * /g4sipm/noise/thermal
 * /g4sipm/noise/preThermal
 * /g4sipm/noise/postThermal
 * /g4sipm/noise/afterpulse
 * /g4sipm/noise/crosstalk
 * /g4sipm/noise/crosstalkNeighbours
 * /g4sipm/noise/ifNoSignal
 * /g4sipm/digitize/hits
 * /g4sipm/digitize/trace
 * /g4sipm/digitize/shuntresistor
 * /g4sipm/digitize/shuntresistorRecoveryTime
 *
 * Documentation of the parameters can be retreived at the corresponding getter methods.
 */
class G4SipmUiMessenger: public G4UImessenger {
private:
	int verbose;
	double seed;
	bool filterGeometry;
	bool filterPde;
	bool filterTiming;
	bool noiseThermal;
	unsigned int noisePreThermal;
	unsigned int noisePostThermal;
	bool noiseAfterpulse;
	bool noiseCrosstalk;
	unsigned int noiseCrosstalkNeighbours;
	bool noiseIfNoSignal;
	bool digitizeHits;
	bool digitizeTrace;
	bool simulateShuntresistor;
	double shuntresistorRecoveryTime;

	G4UIcmdWithAnInteger* verboseCmd;
	G4UIcmdWithADouble* seedCmd;
	G4UIcmdWithABool* filterGeometryCmd;
	G4UIcmdWithABool* filterPdeCmd;
	G4UIcmdWithABool* filterTimingCmd;
	G4UIcmdWithABool* noiseThermalCmd;
	G4UIcmdWithAnInteger* noisePreThermalCmd;
	G4UIcmdWithAnInteger* noisePostThermalCmd;
	G4UIcmdWithABool* noiseAfterpulseCmd;
	G4UIcmdWithABool* noiseCrosstalkCmd;
	G4UIcmdWithAnInteger* noiseCrosstalkNeighboursCmd;
	G4UIcmdWithABool* noiseIfNoSignalCmd;
	G4UIcmdWithABool* digitizeHitsCmd;
	G4UIcmdWithABool* digitizeTraceCmd;
	G4UIcmdWithABool* simulateShuntresistorCmd;
	G4UIcmdWithADoubleAndUnit* shuntresistorRecoveryTimeCmd;

	G4SipmUiMessenger();

	void initRandomEngine();

public:

	/**
	 * The file name of the macro which will be loaded if it exists.
	 */
	static const std::string MACRO_FILE_NAME;

	/**
	 * @return G4SipmUiMessenger - the singleton.
	 */
	static G4SipmUiMessenger* getInstance();

	virtual ~G4SipmUiMessenger();

	/**
	 * Sets default values for all properties.
	 */
	void setDefaults();

	virtual void SetNewValue(G4UIcommand*, G4String);

	/**
	 * @return bool - true if hits should be filtered according to cell geometry.
	 */
	bool isFilterGeometry() const {
		return filterGeometry;
	}

	/**
	 * @return bool - filter hits according to the photon detection efficiency (dicing).
	 */
	bool isFilterPde() const {
		return filterPde;
	}

	/**
	 * @return bool - filter digis according to recovery time of the SiPM (dicing).
	 */
	bool isFilterTiming() const {
		return filterTiming;
	}

	/**
	 * @return bool - generate afterpulses.
	 */
	bool isNoiseAfterpulse() const {
		return noiseAfterpulse;
	}

	/**
	 * @return bool - generate crosstalk.
	 */
	bool isNoiseCrosstalk() const {
		return noiseCrosstalk;
	}

	/**
	 * @return unsigned int - the number of crosstalk neighbours.
	 */
	unsigned int getNoiseCrosstalkNeighbours() const {
		return noiseCrosstalkNeighbours;
	}

	/**
	 * @return bool - generate noise if no photon has hit.
	 */
	bool isNoiseIfNoSignal() const {
		return noiseIfNoSignal;
	}

	/**
	 * @return unsigned int - thermal noise per SiPM cell after signal.
	 */
	unsigned int getNoisePostThermal() const {
		return noisePostThermal;
	}

	/**
	 * @return unsigned int - thermal noise per SiPM cell before signal.
	 */
	unsigned int getNoisePreThermal() const {
		return noisePreThermal;
	}

	/**
	 * @return bool - true if thermal noise should be created.
	 */
	bool isNoiseThermal() const {
		return noiseThermal;
	}

	/**
	 * @return int - verbosity.
	 */
	int getVerbose() const {
		return verbose;
	}

	/**
	 * @return long - the seed
	 */
	long getSeed() const {
		return (long) seed;
	}
	/**
	 * @return bool - true if the hits are to be digitized.
	 */
	bool isDigitizeHits() const {
		return digitizeHits;
	}

	/**
	 * @return bool - true if the trace is to be digitized.
	 */
	bool isDigitizeTrace() const {
		return digitizeTrace;
	}

	/**
	 * @return bool - true if a shunt resistor readout is to be simulated.
	 */
	bool isSimulateShuntresistor() const {
		return simulateShuntresistor;
	}

	/**
	 * @return double - the recovery time constant of a 1 p.e. SiPM pulse read out via the shunt resistor.
	 */
	double getShuntresistorRecoveryTime() const {
		return shuntresistorRecoveryTime;
	}
};

#endif /* G4SIPMDETECTORFILTERMESSENGER_HH_ */
