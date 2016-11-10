/*
 * G4SipmGenericSipmModel.hh
 *
 * @created Apr 23, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMGENERICSIPMMODEL_HH_
#define G4SIPMGENERICSIPMMODEL_HH_

#include "model/G4SipmModel.hh"

/**
 * Generic model implementation with typical values configured.
 */
class G4SipmGenericSipmModel: public G4SipmModel {
public:
	/**
	 * Constructor.
	 */
	G4SipmGenericSipmModel();

	virtual std::string getName() const;
	virtual double getBreakdownVoltage() const;
	virtual unsigned int getNumberOfCells() const;
	virtual double getCellPitch() const;
	virtual double getThermalNoiseRate() const;
	virtual double getDeadTime() const;
	virtual double getRecoveryTime() const;
	virtual double getCrossTalkProbability() const;
	virtual double getApProbLong() const;
	virtual double getApProbShort() const;
	virtual double getApTauLong() const;
	virtual double getApTauShort() const;
	virtual double getFillFactor() const;
	virtual double getGainVariation() const;
	virtual double getPhotonDetectionEfficiency(double wavelength) const;
};

#endif /* G4SIPMGENERICSIPMMODEL_HH_ */
