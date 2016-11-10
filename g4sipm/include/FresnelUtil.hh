/*
 * FresnelUtil.hh
 *
 * @created Oct 5, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef FRESNELUTIL_HH_
#define FRESNELUTIL_HH_

#include <complex>

/**
 * Utility methods for Fresnel refraction.
 */
class FresnelUtil {
private:
	FresnelUtil();

public:
	/**
	 * @param n1 - the index of refraction of medium one.
	 * @param n2 - the index of refraction of medium two.
	 * @param theta - the incident angle.
	 *
	 * @return double - the angle of refraction.
	 */
	static double eta(std::complex<double> n1, std::complex<double> n2, double theta);

	/**
	 * @param n1 - the index of refraction of medium one.
	 * @param n2 - the index of refraction of medium two.
	 * @param theta - the incident angle.
	 *
	 * @return double - the reflectivity of the parallel light component.
	 */
	static double rP(std::complex<double> n1, std::complex<double> n2, double theta);
	static double tP(std::complex<double> n1, std::complex<double> n2, double theta);

	/**
	 * @param n1 - the index of refraction of medium one.
	 * @param n2 - the index of refraction of medium two.
	 * @param theta - the incident angle.
	 *
	 * @return double - the reflectivity of the perpendicular light component.
	 */
	static double rS(std::complex<double> n1, std::complex<double> n2, double theta);
	static double tS(std::complex<double> n1, std::complex<double> n2, double theta);

	/**
	 * @param n1 - the index of refraction of medium one.
	 * @param n2 - the index of refraction of medium two.
	 * @param theta - the incident angle.
	 *
	 * @return double - the reflectivity for unpolarized light.
	 */
	static double r(std::complex<double> n1, std::complex<double> n2, double theta);

	/**
	 * @param n1 - the index of refraction of medium one.
	 * @param n2 - the index of refraction of medium two.
	 * @param theta - the incident angle.
	 *
	 * @return double - the transmission for unpolarized light.
	 */
	static double t(std::complex<double> n1, std::complex<double> n2, double theta);

	/**
	 * @param n1 - the index of refraction of medium one.
	 * @param n2 - the index of refraction of medium two.
	 * @param n3 - the index of refraction of medium three.
	 * @param theta - the incident angle.
	 *
	 * @return double - the transmission for parallel polarized light.
	 */
	static double tP(std::complex<double> n1, std::complex<double> n2, std::complex<double> n3, double theta);

	/**
	 * @param n1 - the index of refraction of medium one.
	 * @param n2 - the index of refraction of medium two.
	 * @param n3 - the index of refraction of medium three.
	 * @param theta - the incident angle.
	 *
	 * @return double - the transmission for perpendicularly polarized light.
	 */
	static double tS(std::complex<double> n1, std::complex<double> n2, std::complex<double> n3, double theta);

	/**
	 * @param n1 - the index of refraction of medium one.
	 * @param n2 - the index of refraction of medium two.
	 * @param n3 - the index of refraction of medium three.
	 * @param theta - the incident angle.
	 *
	 * @return double - the transmission for unpolarized light.
	 */
	static double t(std::complex<double> n1, std::complex<double> n2, std::complex<double> n3, double theta);

};

#endif /* FRESNELUTIL_HH_ */
