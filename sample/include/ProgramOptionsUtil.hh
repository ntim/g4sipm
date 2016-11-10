/*
 * ProgramOptionsUtil.hh
 *
 * @date Apr 12, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef PROGRAMOPTIONSUTIL_HH_
#define PROGRAMOPTIONSUTIL_HH_

#include <boost/program_options.hpp>

namespace po = boost::program_options;

/**
 * Utility methods for Boost program options.
 */
class ProgramOptionsUtil {
private:
	/**
	 * Private constructor.
	 */
	ProgramOptionsUtil();

public:

	/**
	 * Removes unregistered options. This is necessary due to non-compliances of Boost 1.33.1
	 * delivered by Scientific Linux.
	 *
	 * @param parsed - the parsed program options.
	 */
	static void removeUnregisterd(po::parsed_options* parsed);

	/**
	 * Parses program options.
	 *
	 * @param argc - number of arguments.
	 * @param argv - the arguments.
	 * @param desc - options descriptions.
	 * @param allowUnregistered - set true to strip unregistered options (true).
	 * @return variables_map - the variables map.
	 */
	static po::variables_map parse(int argc, char** argv, const po::options_description& desc, bool allowUnregistered =
			true);
};

#endif /* PROGRAMOPTIONSUTIL_HH_ */
