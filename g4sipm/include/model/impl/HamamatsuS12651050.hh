/*
 * HamamatsuS12651050.hh
 *
 * @date Oct 16, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUS12651050_HH_
#define HAMAMATSUS12651050_HH_

#include <model/G4SipmModel.hh>

#include <vector>
#include <string>

class HamamatsuS12651050: public G4SipmModel {
private:
	std::vector<double> wvl;
	std::vector<double> pde;

public:
	HamamatsuS12651050();

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

#endif /* HAMAMATSUS12651050_HH_ */
