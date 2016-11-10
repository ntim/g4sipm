/*
 * G4SipmConfigFileModel.hh
 *
 * @date Mar 13, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMCONFIGFILEMODEL_HH_
#define G4SIPMCONFIGFILEMODEL_HH_

#include <boost/tuple/tuple.hpp>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "Properties.hh"
#include "model/G4SipmModel.hh"

/**
 * SiPM model based on a to be specified config file. The resources directory contains an example file.
 */
class G4SipmConfigFileModel: public G4SipmModel {
private:
	typedef boost::tuple<unsigned int, unsigned int> operatingpoint;

	Properties p;
	std::string name;
	int numberOfCells;
	double cellPitch;
	double fillFactor;
	double thickness;
	double windowThickness;
	G4Material* windowMaterial;
	double gainVariation;
	double deadTime;
	std::vector<double> entry;
	std::vector<double> temperature;
	std::vector<double> breakDownVoltage;
	std::vector<double> overVoltage;
	std::vector<std::vector<double> > pdeWvl;
	std::vector<std::vector<double> > pdeEff;
	std::vector<double> thermalNoiseRate;
	std::vector<double> crossTalkProbability;
	std::vector<double> afterPulseProbLong;
	std::vector<double> afterPulseProbShort;
	std::vector<double> afterPulseTauLong;
	std::vector<double> afterPulseTauShort;
	std::vector<double> recoveryTime;
	operatingpoint op;

protected:
	void initialize();
	void initializePde();
	void initializeWindowMaterial();
	/**
	 * @param quantity - the quantity to interpolate.
	 * @return double - the interpolated quantity.
	 */
	virtual double interpolate(const std::vector<double>& quantity) const;
	/**
	 * Updates the operating point according to the temperature, the bias- and overvoltage.
	 */
	void updateOperatingPoint();

public:
	/**
	 * Constructor.
	 *
	 * @param filename - the filename of the config file.
	 */
	G4SipmConfigFileModel(std::string filename = "sipm.properties");

	virtual void setBiasVoltage(double biasVoltage);
	virtual void setTemperature(double temperature);

	virtual std::string getName() const;
	virtual unsigned int getNumberOfCells() const;
	virtual double getCellPitch() const;
	virtual double getFillFactor() const;
	virtual double getThickness() const;
	virtual G4Material* getWindowMaterial() const;
	virtual double getWindowThickness() const;
	virtual double getBreakdownVoltage() const;
	virtual double getPhotonDetectionEfficiency(double wavelength) const;
	virtual double getThermalNoiseRate() const;
	virtual double getCrossTalkProbability() const;
	virtual double getApProbLong() const;
	virtual double getApProbShort() const;
	virtual double getApTauLong() const;
	virtual double getApTauShort() const;
	virtual double getRecoveryTime() const;
	virtual double getDeadTime() const;
	virtual double getGainVariation() const;
};

#endif /* G4SIPMCONFIGFILEMODEL_HH_ */
