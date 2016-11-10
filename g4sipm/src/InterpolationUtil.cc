/*
 * InterpolationUtil.cc
 *
 * @created Jan 17, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "InterpolationUtil.hh"

#include <algorithm>

InterpolationUtil::InterpolationUtil() {
	//
}

double InterpolationUtil::linear(const std::vector<double>& x, const std::vector<double>& y, const double xi) {
	typedef std::vector<double>::const_iterator i_t;
	i_t i = std::upper_bound(x.begin(), x.end(), xi);
	if (i == x.end()) {
		--i;
	}
	if (i == x.begin()) {
		++i;
	}
	i_t l = i;
	--l;
	const double delta = (xi - *l) / (*i - *l);
	return delta * y.at(std::distance(x.begin(), i)) + (1 - delta) * y.at(std::distance(x.begin(), l));
}

double InterpolationUtil::linear(const std::map<double, double>& xy, const double xi) {
	typedef std::map<double, double>::const_iterator i_t;
	i_t i = xy.upper_bound(xi);
	if (i == xy.end()) {
		--i;
	}
	if (i == xy.begin()) {
		++i;
	}
	i_t l = i;
	--l;
	const double delta = (xi - l->first) / (i->first - l->first);
	return delta * i->second + (1 - delta) * l->second;
}
