/*
 * VectorUtil.cc
 *
 * @created Jan 17, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "VectorUtil.hh"

VectorUtil::VectorUtil() {
	//
}

std::vector<double> VectorUtil::adapt(const double a[], const int size) {
	return std::vector<double>(a, a + size);
}

std::vector<double> VectorUtil::times(const std::vector<double> v, const double scalar) {
	std::vector<double> result(v);
	for (std::vector<double>::iterator iter = result.begin(); iter != result.end(); iter++) {
		(*iter) *= scalar;
	}
	return result;
}

double VectorUtil::min(std::vector<double> v) {
	double m = v.front();
	for (std::vector<double>::iterator iter = v.begin(); iter != v.end(); iter++) {
		m = std::min(m, *iter);
	}
	return m;
}

double VectorUtil::max(std::vector<double> v) {
	double m = v.front();
	for (std::vector<double>::iterator iter = v.begin(); iter != v.end(); iter++) {
		m = std::max(m, *iter);
	}
	return m;
}

double VectorUtil::mean(std::vector<double> v) {
	double m = 0;
	for (std::vector<double>::iterator iter = v.begin(); iter != v.end(); iter++) {
		m += *iter;
	}
	return m / v.size();
}

void VectorUtil::expand(std::map<double, double> tabular, std::vector<double>* x, std::vector<double>* y) {
	for (std::map<double, double>::iterator iter = tabular.begin(); iter != tabular.end(); iter++) {
		x->push_back(iter->first);
		y->push_back(iter->second);
	}
}

void VectorUtil::reverse(double* a, const int n) {
	for (int i = 0; i < n / 2; i++) {
		double tmp = a[i];
		a[i] = a[n - i - 1];
		a[n - i - 1] = tmp;
	}
}

std::vector<double> VectorUtil::add(const std::vector<double>& v, const double c) {
	std::vector<double> r;
	for (std::vector<double>::const_iterator it = v.begin(); it != v.end(); it++) {
		r.push_back(*it + c);
	}
	return r;
}
