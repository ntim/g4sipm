/*
 * G4SipmVoltageTrace.hh
 *
 * @created Apr 19, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMVOLTAGETRACE_HH_
#define G4SIPMVOLTAGETRACE_HH_

#include <vector>

#include <G4VDigi.hh>
#include <G4Allocator.hh>
#include <G4TDigiCollection.hh>

#include <CLHEP/Units/SystemOfUnits.h>

#include "G4Sipm.hh"

/**
 * This represents a single trace created as a result of multiple cell triggers (G4SipmDigi) by
 * the electronics.
 */
class G4SipmVoltageTraceDigi: public G4VDigi {
private:
	G4SipmId sipmId;
	double tMin;
	double tMax;
	double timeBinWidth;
	int precision;
	double voltageRange;
	std::vector<double> voltages;

public:
	/**
	 * Constructor.
	 *
	 * @param sipmId - the SiPM id.
	 * @param tMin - the minimum trace time.
	 * @param tMax - the maximum trace time.
	 * @param timeBinWidth - the time bin widht.
	 */
	G4SipmVoltageTraceDigi(const G4SipmId sipmId, const double tMin, const double tMax,
			const double timeBinWidth = 1. * CLHEP::nanosecond);
	virtual ~G4SipmVoltageTraceDigi();

	const G4SipmVoltageTraceDigi& operator=(const G4SipmVoltageTraceDigi&);
	int operator==(const G4SipmVoltageTraceDigi&) const;
	inline void* operator new(size_t);
	inline void operator delete(void* aDigi);

	/**
	 * @param t - a time.
	 * @return size_t - the index corresponding to the time.
	 */
	size_t index(const double t) const;
	/**
	 * @param i - an index.
	 * @return double - the time corresponding to the index.
	 */
	double time(const size_t i) const;
	/**
	 * @param i - an index.
	 * @return double - reference to the voltage trace value.
	 */
	double& at(size_t i);
	/**
	 * @param i - an index.
	 * @return double - the voltage trace value respecting a limited precision.
	 */
	double atMeasured(size_t i) const;
	size_t size() const;

	/**
	 * @return G4SipmId - the SiPM id.
	 */
	G4SipmId getSipmId() const;
	/**
	 * @param sipmId - the SiPM id to set.
	 */
	void setSipmId(G4SipmId sipmId);
	/**
	 * @return int - the measurement precision in bit.
	 */
	int getPrecision() const;
	/**
	 * @param precision - the measurement precision in bit to set.
	 */
	void setPrecision(int precision);
	/**
	 * @return double - the time bin with.
	 */
	double getTimeBinWidth() const;
	/**
	 * @return double - the maximum trace time.
	 */
	double getTMax() const;
	/**
	 * @return double - the minimum trace time.
	 */
	double getTMin() const;
	/**
	 * @return double - the voltage range which is used to calculate the channel width for a measurement.
	 */
	double getVoltageRange() const;
	/**
	 * @param voltageRange - the voltage range to set which is used to calculate the channel width for a measurement.
	 */
	void setVoltageRange(double voltageRange);
	/**
	 * @return vector - the underlying container object of the voltage values.
	 */
	std::vector<double> getContainer() const;
};

typedef G4TDigiCollection<G4SipmVoltageTraceDigi> G4SipmVoltageTraceDigiCollection;

extern G4ThreadLocal G4Allocator<G4SipmVoltageTraceDigi>* G4SipmVoltageTraceDigiAllocator;

inline void* G4SipmVoltageTraceDigi::operator new(size_t) {
	if (!G4SipmVoltageTraceDigiAllocator) {
		G4SipmVoltageTraceDigiAllocator = new G4Allocator<G4SipmVoltageTraceDigi>;
	}
	return G4SipmVoltageTraceDigiAllocator->MallocSingle();
}

inline void G4SipmVoltageTraceDigi::operator delete(void* aDigi) {
	G4SipmVoltageTraceDigiAllocator->FreeSingle((G4SipmVoltageTraceDigi*) aDigi);
}

#endif /* G4SIPMVOLTAGETRACE_HH_ */
