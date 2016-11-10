/*
 * G4SipmModel.hh
 *
 * @created Apr 11, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMMODEL_HH_
#define G4SIPMMODEL_HH_

#include <string>

#include <CLHEP/Vector/TwoVector.h>

#include <G4Material.hh>

#include "G4SipmCellId.hh"
#include "model/G4SipmGainMapModel.hh"
#include "model/G4SipmVoltageTraceModel.hh"

/**
 * SiPM model.
 */
class G4SipmModel {
private:
	double biasVoltage;
	double temperature;

	G4SipmGainMapModel* gainMapModel;
	G4SipmVoltageTraceModel* voltageTraceModel;

	/**
	 * @return G4SipmCellId - an invalid id.
	 */
	G4SipmCellId getInvalidCellId() const;

public:
	/**
	 * Constructor.
	 *
	 * @param gainMapModel - the gain map model.
	 * @param voltageTraceModel - the voltage trace model.
	 */
	G4SipmModel(G4SipmGainMapModel* gainMapModel, G4SipmVoltageTraceModel* voltageTraceModel);
	virtual ~G4SipmModel();

	/**
	 * @return double - the complete pitch of the SiPM.
	 */
	double getPitch() const;
	/**
	 * @param cellId - the id of the cell.
	 * @return Hep2Vector - the position relative to the center.
	 */
	CLHEP::Hep2Vector getCellPosition(G4SipmCellId cellId) const;
	/**
	 * @param x - the x coordinate.
	 * @param y - the y coordinate.
	 * @param respectFillFactor - set true to return an invalid cell id if the coordinates are not in an active area.
	 * @return G4SipmCellId - the corresponding cell id.
	 */
	G4SipmCellId getCellId(const double x, const double y, bool respectFillFactor = false) const;
	/**
	 * @param cellId - the cellId.
	 * @return bool - true if the cellId is valid.
	 */
	bool isValidCellId(const G4SipmCellId cellId) const;
	/**
	 * @param cellId - the cellId.
	 * @return double - the gain of the cell.
	 */
	double getGain(const G4SipmCellId cellId) const;
	/**
	 * @return double - the current overvoltage.
	 */
	double getOverVoltage() const;

	/**
	 * @return string - the name of the model.
	 */
	virtual std::string getName() const = 0;
	/**
	 * @return double - the current breakdown voltage.
	 */
	virtual double getBreakdownVoltage() const = 0;
	/**
	 * @return unsigned int - the number of cells.
	 */
	virtual unsigned int getNumberOfCells() const = 0;
	/**
	 * @return double - the pitch of a single cell.
	 */
	virtual double getCellPitch() const = 0;
	/**
	 * @return double - the thermal noise rate.
	 */
	virtual double getThermalNoiseRate() const = 0;
	/**
	 * @return double - the dead time of a SiPM cell.
	 */
	virtual double getDeadTime() const = 0;
	/**
	 * @return double - the recovery time of the SiPM cell gain.
	 */
	virtual double getRecoveryTime() const = 0;
	/**
	 * @return double - the crosstalk probability.
	 */
	virtual double getCrossTalkProbability() const = 0;
	/**
	 * @return double - the probability for long time constant afterpulses.
	 */
	virtual double getApProbLong() const = 0;
	/**
	 * @return double - the probability for short time constant afterpulses.
	 */
	virtual double getApProbShort() const = 0;
	/**
	 * @return double - the time constant of long time constant afterpulses.
	 */
	virtual double getApTauLong() const = 0;
	/**
	 * @return double - the time constant of short time constant afterpulses.
	 */
	virtual double getApTauShort() const = 0;
	/**
	 * @return double - the fill factor.
	 */
	virtual double getFillFactor() const = 0;
	/**
	 * @return double - the gain variance.
	 */
	virtual double getGainVariation() const = 0;
	/**
	 * @param wavelength - the photon wavelength.
	 * @return double - the photon detection efficiency.
	 */
	virtual double getPhotonDetectionEfficiency(double wavelength) const = 0;

	/**
	 * @return G4Material - the material of the chip.
	 */
	virtual G4Material* getMaterial() const;
	/**
	 * @return double - the thickness of the chip.
	 */
	virtual double getThickness() const;
	/**
	 * @return G4Material - the material of the window.
	 */
	virtual G4Material* getWindowMaterial() const;
	/**
	 * @return double - the window thickness.
	 */
	virtual double getWindowThickness() const;

	/**
	 * @return double - the current temperature of the chip.
	 */
	double getTemperature() const;
	/**
	 * @param temperature - the temperature to set.
	 */
	virtual void setTemperature(double temperature);
	/**
	 * @return double - the bias voltage.
	 */
	double getBiasVoltage() const;
	/**
	 * @param biasVoltage - the biasVoltage to set.
	 */
	virtual void setBiasVoltage(double biasVoltage);
	/**
	 * @return G4SipmGainMapModel - the gain map model.
	 */
	G4SipmGainMapModel* getGainMapModel() const;
	/**
	 * @return G4SipmVoltageTraceModel - the voltage trace model.
	 */
	G4SipmVoltageTraceModel* getVoltageTraceModel() const;
};

#endif /* G4SIPMMODEL_HH_ */
