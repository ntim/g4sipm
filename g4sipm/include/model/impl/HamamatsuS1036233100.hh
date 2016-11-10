/*
 * HamamatsuS1036233100.hh
 *
 * @created Apr 24, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUS1036233100_HH_
#define HAMAMATSUS1036233100_HH_

#include "model/impl/HamamatsuS1036211100.hh"

/**
 * Implementation of the Hamamatsu S10362-33-100 series MPPC.
 */
class HamamatsuS1036233100: public HamamatsuS1036211100 {
public:
	/**
	 * Constructor.
	 */
	HamamatsuS1036233100();

	virtual std::string getName() const;
	virtual unsigned int getNumberOfCells() const;
	virtual double getThermalNoiseRate() const;
};

#endif /* HAMAMATSUS1036233100_HH_ */
