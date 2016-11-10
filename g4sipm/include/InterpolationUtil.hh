/*
 * InterpolationUtil.hh
 *
 * @date Feb 14, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef INTERPOLATIONUTIL_HH_
#define INTERPOLATIONUTIL_HH_

#include <vector>
#include <map>

/**
 * Utility class implementing interpolation between discrete 2d data.
 */
class InterpolationUtil {
private:
	InterpolationUtil();

public:
	/**
	 * Linear interpolation.
	 *
	 * @param x - the x vector.
	 * @param y - the y vector.
	 * @param xi - the point to interpolate.
	 */
	static double linear(const std::vector<double>& x, const std::vector<double>& y, const double xi);
	/**
	 * Linear interpolation.
	 *
	 * @param xy - the map containing x (key) and y (value).
	 * @param xi - the point to interpolate.
	 */
	static double linear(const std::map<double, double>& xy, const double xi);

};

#endif /* INTERPOLATIONUTIL_HH_ */
