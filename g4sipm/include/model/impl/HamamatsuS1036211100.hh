/*
 * HamamatsuS1036211100.hh
 *
 * @created Apr 24, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUS1036211100_HH_
#define HAMAMATSUS1036211100_HH_

#include "model/G4SipmModel.hh"

/**
 * Implementation of the Hamamatsu S10362-11-100 series MPPC.
 */
class HamamatsuS1036211100: public G4SipmModel {
private:
	std::vector<double> wvl;
	std::vector<double> pde;

public:
	HamamatsuS1036211100();

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

#endif /* HAMAMATSUS1036211100_HH_ */
