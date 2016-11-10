/*
 * G4SipmVoltageTraceModel.hh
 *
 * @created Apr 19, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMVOLTAGETRACEMODEL_HH_
#define G4SIPMVOLTAGETRACEMODEL_HH_

/**
 * Voltage trace model.
 *
 * TODO: parameters should be overvoltage dependent.
 */
class G4SipmVoltageTraceModel {
private:
	/**
	 * Pulse shape with a linear rising edge and an exponential falling edge.
	 *
	 * @param t - the time.
	 * @param gain - the gain.
	 * @return double - the current amplitude.
	 */
	double singleExponential(const double t, const double gain) const;
	/**
	 * Pulse shape with an exponential rising and falling edge. Features a smooth transition between the two components.
	 *
	 * @param t - the time.
	 * @param gain - the gain.
	 * @return double - the current amplitude.
	 */
	double doubleExponential(const double t, const double gain) const;

public:
	/**
	 * Constructor.
	 */
	G4SipmVoltageTraceModel();
	virtual ~G4SipmVoltageTraceModel();

	/**
	 * Pulse parameterization (double exponential by default).
	 *
	 * @param t - the time after trigger.
	 * @param gain - the gain in percent.
	 * @return double - the current amplitude.
	 */
	double pulse(const double t, const double gain = 1.) const;

	/**
	 * @return double - the amplitude of the pulse.
	 */
	virtual double getAmplitude() const = 0;
	/**
	 * @return double - the rising edge time constant.
	 */
	virtual double getTauRise() const = 0;
	/**
	 * @return double - the falling edge time constant.
	 */
	virtual double getTauFall() const = 0;
	/**
	 * @return double - the baseline height.
	 */
	virtual double getV0() const = 0;
	/**
	 * @return double - the white noise variance of the baseline.
	 */
	virtual double getWhiteNoiseSigma() const = 0;
	/**
	 * @return int - the precision in bit.
	 */
	virtual int getPrecision() const = 0;
	/**
	 * @return double - the time bin width.
	 */
	virtual double getTimeBinWidth() const = 0;
};

#endif /* G4SIPMVOLTAGETRACEMODEL_HH_ */
