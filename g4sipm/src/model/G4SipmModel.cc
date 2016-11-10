/*
 * SipmModel.cc
 *
 * @created Apr 11, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/G4SipmModel.hh"

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include "MaterialFactory.hh"
#include "model/impl/G4SipmRandomGainMapModel.hh"

G4SipmModel::G4SipmModel(G4SipmGainMapModel* _gainMapModel, G4SipmVoltageTraceModel* _voltageTraceModel) :
		biasVoltage(70. * CLHEP::volt), temperature(20. * CLHEP::kelvin + CLHEP::STP_Temperature), gainMapModel(
				_gainMapModel), voltageTraceModel(_voltageTraceModel) {
	//
}

G4SipmModel::~G4SipmModel() {
	delete gainMapModel;
	delete voltageTraceModel;
}

double G4SipmModel::getPitch() const {
	return sqrt(getNumberOfCells()) * getCellPitch();
}

CLHEP::Hep2Vector G4SipmModel::getCellPosition(G4SipmCellId cellId) const {
	int sqrtN = static_cast<int>(sqrt(static_cast<double>(getNumberOfCells())));
	//
	int j = static_cast<int>((cellId % sqrtN));
	int i = (cellId - j) / sqrtN;
	// Derive cell center:
	double xC = (i + 1. / 2.) * getCellPitch() - getPitch() / 2.;
	double yC = (j + 1. / 2.) * getCellPitch() - getPitch() / 2.;
	return CLHEP::Hep2Vector(xC, yC);
}

G4SipmCellId G4SipmModel::getCellId(double x, double y, bool respectFillFactor) const {
	int sqrtN = static_cast<int>(sqrt(static_cast<double>(getNumberOfCells())));
	// Derive cell i,j.
	int i = static_cast<int>(floor((x + getPitch() / 2.) / getCellPitch()));
	int j = static_cast<int>(floor((y + getPitch() / 2.) / getCellPitch()));
	// Check if it is within the range.
	if (i < 0 || j < 0 || i >= sqrtN || j >= sqrtN) {
		return getInvalidCellId();
	}
	// Return if now fill factor check is required.
	if (!respectFillFactor) {
		return i * sqrtN + j;
	}
	// Derive cell center:
	double xC = (i + 1. / 2.) * getCellPitch() - getPitch() / 2.;
	double yC = (j + 1. / 2.) * getCellPitch() - getPitch() / 2.;
	// Derive bounds
	double x1 = xC - getCellPitch() / 2. * sqrt(getFillFactor());
	double y1 = yC - getCellPitch() / 2. * sqrt(getFillFactor());
	double x2 = xC + getCellPitch() / 2. * sqrt(getFillFactor());
	double y2 = yC + getCellPitch() / 2. * sqrt(getFillFactor());
	if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {
		return i * sqrtN + j;
	}
	return getInvalidCellId();
}

G4SipmCellId G4SipmModel::getInvalidCellId() const {
	return getNumberOfCells();
}

bool G4SipmModel::isValidCellId(const G4SipmCellId cellId) const {
	return cellId < getNumberOfCells();
}

double G4SipmModel::getGain(const G4SipmCellId cellId) const {
	if (gainMapModel->needsRefresh()) {
		gainMapModel->refresh(this);
	}
	return gainMapModel->getGain(cellId);
}

G4SipmGainMapModel* G4SipmModel::getGainMapModel() const {
	return gainMapModel;
}

G4SipmVoltageTraceModel* G4SipmModel::getVoltageTraceModel() const {
	return voltageTraceModel;
}

double G4SipmModel::getThickness() const {
	return .1 * CLHEP::mm;
}

G4Material* G4SipmModel::getMaterial() const {
	return MaterialFactory::getInstance()->getSilicon();
}

G4Material* G4SipmModel::getWindowMaterial() const {
	return MaterialFactory::getInstance()->getEpoxy();
}

double G4SipmModel::getTemperature() const {
	return temperature;
}

double G4SipmModel::getOverVoltage() const {
	return getBiasVoltage() - getBreakdownVoltage();
}

double G4SipmModel::getBiasVoltage() const {
	return biasVoltage;
}

void G4SipmModel::setBiasVoltage(double _biasVoltage) {
	biasVoltage = _biasVoltage;
}

void G4SipmModel::setTemperature(double _temperature) {
	temperature = _temperature;
}

double G4SipmModel::getWindowThickness() const {
	return .1 * CLHEP::mm;
}
