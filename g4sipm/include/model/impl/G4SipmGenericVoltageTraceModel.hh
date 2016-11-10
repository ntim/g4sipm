/*
 * G4SipmGenericVoltageTraceModel.hh
 *
 * @created Apr 30, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMGENERICVOLTAGETRACEMODEL_HH_
#define G4SIPMGENERICVOLTAGETRACEMODEL_HH_

#include "model/G4SipmVoltageTraceModel.hh"

/**
 * Generic model implementation with typical values configured.
 */
class G4SipmGenericVoltageTraceModel: public G4SipmVoltageTraceModel {
public:
	/**
	 * Constructor.
	 */
	G4SipmGenericVoltageTraceModel();

	virtual double getAmplitude() const;
	virtual double getTauRise() const;
	virtual double getTauFall() const;
	virtual double getV0() const;
	virtual double getWhiteNoiseSigma() const;
	virtual int getPrecision() const;
	virtual double getTimeBinWidth() const;
};

#endif /* G4SIPMGENERICVOLTAGETRACEMODEL_HH_ */
