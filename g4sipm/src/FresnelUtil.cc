/*
 * FresnelUtil.cc
 *
 * @created Jan 17, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "FresnelUtil.hh"

#include <math.h>

FresnelUtil::FresnelUtil() {
	//
}

double FresnelUtil::eta(std::complex<double> n1, std::complex<double> n2, double theta) {
	return std::asin(n1.real() / n2.real() * std::sin(theta));
}

double FresnelUtil::rP(std::complex<double> n1, std::complex<double> n2, double theta) {
	const double beta = eta(n1, n2, theta);
	std::complex<double> nom = n2 * std::cos(theta) - n1 * std::cos(beta);
	std::complex<double> denom = n2 * std::cos(theta) + n1 * std::cos(beta);
	return std::abs(nom / denom) * std::abs(nom / denom);
}

double FresnelUtil::tP(std::complex<double> n1, std::complex<double> n2, double theta) {
	const double beta = eta(n1, n2, theta);
	std::complex<double> nom = 2. * n1 * std::cos(theta);
	std::complex<double> denom = n2 * std::cos(theta) + n1 * std::cos(beta);
	return std::abs(n2 * std::cos(beta) / (n1 * std::cos(theta))) * std::abs(nom / denom) * std::abs(nom / denom);
}

double FresnelUtil::rS(std::complex<double> n1, std::complex<double> n2, double theta) {
	const double beta = eta(n1, n2, theta);
	std::complex<double> nom = n1 * std::cos(theta) - n2 * std::cos(beta);
	std::complex<double> denom = n1 * std::cos(theta) + n2 * std::cos(beta);
	return std::abs(nom / denom) * std::abs(nom / denom);
}

double FresnelUtil::tS(std::complex<double> n1, std::complex<double> n2, double theta) {
	std::complex<double> nom = 2. * n1 * std::cos(theta);
	std::complex<double> denom = n1 * std::cos(theta) + n2 * std::cos(eta(n1, n2, theta));
	return std::abs(nom / denom) * std::abs(nom / denom);
}

double FresnelUtil::r(std::complex<double> n1, std::complex<double> n2, double theta) {
	return (rS(n1, n2, theta) + rP(n1, n2, theta)) / 2.;
}

double FresnelUtil::t(std::complex<double> n1, std::complex<double> n2, double theta) {
	return 1. - r(n1, n2, theta);
}

double FresnelUtil::tP(std::complex<double> n1, std::complex<double> n2, std::complex<double> n3, double theta) {
	double beta = eta(n1, n2, theta);
	double nom = tP(n1, n2, theta) * tP(n2, n3, beta);
	double denom = (1. - rP(n2, n1, beta) * rP(n2, n3, beta));
	return nom / denom;
}

double FresnelUtil::tS(std::complex<double> n1, std::complex<double> n2, std::complex<double> n3, double theta) {
	double beta = eta(n1, n2, theta);
	double nom = tS(n1, n2, theta) * tS(n2, n3, beta);
	double denom = (1. - rS(n2, n1, beta) * rS(n2, n3, beta));
	return nom / denom;
}

double FresnelUtil::t(std::complex<double> n1, std::complex<double> n2, std::complex<double> n3, double theta) {
	double beta = eta(n1, n2, theta);
	double nom = t(n1, n2, theta) * t(n2, n3, beta);
	double denom = (1. - r(n2, n1, beta) * r(n2, n3, beta));
	return (nom / denom);
}

