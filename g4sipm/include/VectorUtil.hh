/*
 * VectorUtil.hh
 *
 * @date Mar 7, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef VECTORUTIL_HH_
#define VECTORUTIL_HH_

#include <vector>
#include <map>
#include <boost/any.hpp>

/**
 * Utility class for operations on STL vectors.
 */
class VectorUtil {
private:
	VectorUtil();

public:
	/**
	 * Adapts an array to an stl-vector.
	 *
	 * @param a - the array.
	 * @param size - the size of the array.
	 * @return std::vector - the stl-vector.
	 * @deprecated
	 */
	static std::vector<double> adapt(const double a[], const int size);

	/**
	 * Adapts an stl-vector of boost::any to an stl-vector of doubles.
	 *
	 * @param v - the array.
	 * @return std::vector - the stl-vector.
	 */
	template<class T> static std::vector<T> adapt(std::vector<boost::any>& v);

	/**
	 * Multiplies all entries of a vector with the given scalar.
	 *
	 * @param v - the vector.
	 * @param scalar - the scalar.
	 */
	static std::vector<double> times(const std::vector<double> v, const double scalar);

	/**
	 * @param v - a vector.
	 * @return double - the smallest entry in v.
	 */
	static double min(std::vector<double> v);

	/**
	 * @param v - a vector.
	 * @return double - the largest entry in v.
	 */
	static double max(std::vector<double> v);

	/**
	 * @param v - a vector.
	 * @return double - the arithmetic mean of all entries in v.
	 */
	static double mean(std::vector<double> v);

	/**
	 * Expands a map into a vector of keys (x) and values (y).
	 * @param tabular - a tabular data in form of a map<double, double>.
	 * @param x - the keys.
	 * @param y - the values.
	 */
	static void expand(std::map<double, double> tabular, std::vector<double>* x, std::vector<double>* y);

	/**
	 * Reverses the order of entries in an array.
	 *
	 * @param a - the array.
	 * @param n - the length of the array.
	 */
	static void reverse(double* a, const int n);

	/**
	 * Adds a constant to all elements of the vector.
	 *
	 * @param v - the array.
	 * @param c - the constant to add.
	 */
	static std::vector<double> add(const std::vector<double>& v, const double c);

};

template<class T> std::vector<T> VectorUtil::adapt(std::vector<boost::any>& v) {
	std::vector<T> result;
	for (std::vector<boost::any>::iterator iter = v.begin(); iter != v.end(); iter++) {
		result.push_back(boost::any_cast<T>(*iter));
	}
	return result;
}

#endif /* VECTORUTIL_HH_ */
