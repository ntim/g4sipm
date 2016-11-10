/*
 * VoltageTrace.cc
 *
 * @created Apr 19, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "digi/G4SipmVoltageTraceDigi.hh"

#include <math.h>

G4ThreadLocal G4Allocator<G4SipmVoltageTraceDigi>* G4SipmVoltageTraceDigiAllocator = 0;

G4SipmVoltageTraceDigi::G4SipmVoltageTraceDigi(const G4SipmId _sipmId, const double _tMin, const double _tMax,
		const double _timeBinWidth) :
		sipmId(_sipmId), tMin(_tMin), tMax(_tMax), timeBinWidth(_timeBinWidth), precision(12), voltageRange(2. * CLHEP::volt) {
	size_t n = ceil((tMax - tMin) / timeBinWidth);
	voltages = std::vector<double>(n, 0.);
}

G4SipmVoltageTraceDigi::~G4SipmVoltageTraceDigi() {
	//
}

const G4SipmVoltageTraceDigi& G4SipmVoltageTraceDigi::operator =(const G4SipmVoltageTraceDigi& right) {
	tMin = right.getTMin();
	tMax = right.getTMax();
	timeBinWidth = right.getTimeBinWidth();
	precision = right.getPrecision();
	voltageRange = right.getVoltageRange();
	voltages = right.getContainer();
	return *this;
}

int G4SipmVoltageTraceDigi::operator ==(const G4SipmVoltageTraceDigi& right) const {
	return tMin == right.getTMin() && tMax == right.getTMax() && timeBinWidth == right.getTimeBinWidth()
			&& precision == right.getPrecision() && voltageRange == right.getVoltageRange()
			&& voltages == right.getContainer();
}

int G4SipmVoltageTraceDigi::getPrecision() const {
	return precision;
}

void G4SipmVoltageTraceDigi::setPrecision(int _precision) {
	precision = _precision;
}

double G4SipmVoltageTraceDigi::getTimeBinWidth() const {
	return timeBinWidth;
}

double G4SipmVoltageTraceDigi::getTMax() const {
	return tMax;
}

double& G4SipmVoltageTraceDigi::at(size_t i) {
	return voltages.at(i);
}

double G4SipmVoltageTraceDigi::atMeasured(size_t i) const {
	double factor = pow(2, precision) / voltageRange;
	return round(voltages.at(i) * factor) / factor;
}

double G4SipmVoltageTraceDigi::getVoltageRange() const {
	return voltageRange;
}

size_t G4SipmVoltageTraceDigi::index(const double t) const {
	return round((t - tMin) / timeBinWidth);
}

size_t G4SipmVoltageTraceDigi::size() const {
	return voltages.size();
}

double G4SipmVoltageTraceDigi::time(const size_t i) const {
	return i * timeBinWidth + tMin;
}

void G4SipmVoltageTraceDigi::setVoltageRange(double _voltageRange) {
	voltageRange = _voltageRange;
}

double G4SipmVoltageTraceDigi::getTMin() const {
	return tMin;
}

G4SipmId G4SipmVoltageTraceDigi::getSipmId() const {
	return sipmId;
}
std::vector<double> G4SipmVoltageTraceDigi::getContainer() const {
	return voltages;
}
