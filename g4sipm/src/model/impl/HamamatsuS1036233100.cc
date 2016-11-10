/*
 * HamamatsuS1036233100.cc
 *
 * @created Apr 24, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/impl/HamamatsuS1036233100.hh"

HamamatsuS1036233100::HamamatsuS1036233100() :
		HamamatsuS1036211100() {
	//
}

std::string HamamatsuS1036233100::getName() const {
	return "HamamatsuS1036233100";
}

unsigned int HamamatsuS1036233100::getNumberOfCells() const {
	return 900;
}

double HamamatsuS1036233100::getThermalNoiseRate() const {
	return 9. * HamamatsuS1036211100::getThermalNoiseRate();
}
