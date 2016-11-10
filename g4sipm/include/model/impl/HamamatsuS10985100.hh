/*
 * HamamatsuS10985100.hh
 *
 * @created Apr 24, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef HAMAMATSUS10985100_HH_
#define HAMAMATSUS10985100_HH_

#include "HamamatsuS1036211100.hh"

/**
 * Implementation of the Hamamatsu S10985-100 series MPPC array.
 */
class HamamatsuS10985100: public HamamatsuS1036211100 {
public:
	/**
	 * Constructor.
	 */
	HamamatsuS10985100();

	virtual std::string getName() const;
	virtual unsigned int getNumberOfCells() const;
	virtual double getThermalNoiseRate() const;
};

#endif /* HAMAMATSUS10985100_HH_ */
