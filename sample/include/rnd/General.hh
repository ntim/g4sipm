/*
 * General.hh
 *
 *  Created on: 22.04.2015
 *      Author: tim
 */

#ifndef RND_GENERAL_HH_
#define RND_GENERAL_HH_

#include "rnd/Random.hh"

#include <vector>
#include <string>

namespace rnd {

class General: public Random<double> {
private:
	std::vector<double> x;
	std::vector<double> pdf;

	void normalizePdf();

public:
	General(std::vector<double> _x, std::vector<double> _pdf);
	General(std::string filename);
	virtual ~General();

	virtual double shoot();
	void print() const;

	double getXMin() const;
	double getXMax() const;
};

} /* namespace rnd */

#endif /* RND_GENERAL_HH_ */
