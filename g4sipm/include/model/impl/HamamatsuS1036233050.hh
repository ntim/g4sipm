/*
 * HamamatsuS1036233050.hh
 *
 * @created Feb 13, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUS1036233050_HH_
#define HAMAMATSUS1036233050_HH_

#include "model/impl/HamamatsuS1036233100.hh"

/**
 * Implementation of the Hamamatsu S10362-33-050 series MPPC.
 */
class HamamatsuS1036233050: public HamamatsuS1036233100 {
public:
	/**
	 * Constructor.
	 */
	HamamatsuS1036233050();

	virtual std::string getName() const;
	virtual double getBreakdownVoltage() const;
	virtual unsigned int getNumberOfCells() const;
	virtual double getCellPitch() const;
	virtual double getThermalNoiseRate() const;
	virtual double getRecoveryTime() const;
	virtual double getDeadTime() const;
	virtual double getCrossTalkProbability() const;
	virtual double getApProbLong() const;
	virtual double getApProbShort() const;
	virtual double getApTauLong() const;
	virtual double getApTauShort() const;
	virtual double getFillFactor() const;
	virtual double getPhotonDetectionEfficiency(double wavelength) const;
};

#endif /* HAMAMATSUS1036233050_HH_ */
