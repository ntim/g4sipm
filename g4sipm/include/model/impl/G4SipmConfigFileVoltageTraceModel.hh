/*
 * G4SipmConfigFileVoltageTraceModel.hh
 *
 * @created Apr 29, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMCONFIGFILEVOLTAGETRACEMODEL_HH_
#define G4SIPMCONFIGFILEVOLTAGETRACEMODEL_HH_

#include <string>

#include "model/G4SipmVoltageTraceModel.hh"

/**
 * Config file driven implementation of the voltage trace model.
 */
class G4SipmConfigFileVoltageTraceModel: public G4SipmVoltageTraceModel {
private:
	double amplitude;
	double tauRise;
	double tauFall;
	double v0;
	double whiteNoiseSigma;
	double timeBinWidth;
	int precision;

public:
	/**
	 * Constructor.
	 *
	 * @param filename - the properties file name.
	 */
	G4SipmConfigFileVoltageTraceModel(std::string filename);

	virtual double getAmplitude() const;
	virtual double getTauRise() const;
	virtual double getTauFall() const;
	virtual double getV0() const;
	virtual double getWhiteNoiseSigma() const;
	virtual int getPrecision() const;
	virtual double getTimeBinWidth() const;
};

#endif /* G4SIPMCONFIGFILEVOLTAGETRACEMODEL_HH_ */
