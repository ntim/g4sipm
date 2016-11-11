/*
 * HamamatsuS12573100C.hh
 *
 * @date Apr 11, 2016
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUS12573100C_HH_
#define HAMAMATSUS12573100C_HH_

#include "model/G4SipmModel.hh"

/**
 * Implementation of the Hamamatsu S12573-100C.
 */
class HamamatsuS12573100C: public G4SipmModel {
private:
	std::vector<double> wvl;
	std::vector<double> pde;
	G4Material* windowMaterial;

	class VoltageTraceModel: public G4SipmVoltageTraceModel {
	public:
		VoltageTraceModel();
		virtual double getAmplitude() const;
		virtual double getTauRise() const;
		virtual double getTauFall() const;
		virtual double getV0() const;
		virtual double getWhiteNoiseSigma() const;
		virtual int getPrecision() const;
		virtual double getTimeBinWidth() const;
	};

public:
	HamamatsuS12573100C();
	virtual ~HamamatsuS12573100C();

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
	virtual G4Material* getWindowMaterial() const;
	virtual double getWindowThickness() const;
};

#endif /* HAMAMATSUS12573100C_HH_ */
