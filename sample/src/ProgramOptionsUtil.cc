/*
 * ProgramOptionsUtil.cc
 *
 * @date Apr 12, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "ProgramOptionsUtil.hh"

ProgramOptionsUtil::ProgramOptionsUtil() {
	//
}

void ProgramOptionsUtil::removeUnregisterd(po::parsed_options* parsed) {
	std::vector<po::basic_option<char> >::iterator iter = parsed->options.begin();
	while (iter != parsed->options.end()) {
		if ((*iter).unregistered) {
			iter = parsed->options.erase(iter);
		} else {
			iter++;
		}
	}
}

po::variables_map ProgramOptionsUtil::parse(int argc, char** argv, const po::options_description& desc,
		bool allowUnregistered) {
	po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
	if (allowUnregistered) {
		removeUnregisterd(&parsed);
	}
	po::variables_map vm;
	po::store(parsed, vm);
	po::notify(vm);
	return vm;
}
