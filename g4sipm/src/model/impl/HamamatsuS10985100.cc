/*
 * HamamatsuS10985100.cc
 *
 * @created Apr 24, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/impl/HamamatsuS10985100.hh"

HamamatsuS10985100::HamamatsuS10985100() :
		HamamatsuS1036211100() {
	//
}

std::string HamamatsuS10985100::getName() const {
	return "HamamatsuS10985100";
}

unsigned int HamamatsuS10985100::getNumberOfCells() const {
	return 3600;
}

double HamamatsuS10985100::getThermalNoiseRate() const {
	return 81. * HamamatsuS1036211100::getThermalNoiseRate();
}
