/*
 * PersistVisitorFactory.hh
 *
 * @date Jul 8, 2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef PERSISTENCY_PERSISTVISITORFACTORY_HH_
#define PERSISTENCY_PERSISTVISITORFACTORY_HH_

#include <string>

#include "PersistVisitor.hh"

class PersistVisitorFactory {
private:
	PersistVisitorFactory();

public:
	virtual ~PersistVisitorFactory();

	static PersistVisitorFactory* getInstance();

	/**
	 * Creates a persist visitor based on the extension of the filename:
	 * *.root : ROOT file (if activated in CMake build settings)
	 * *.sqlite : Sqlite3 database (if activated in CMake build settings)
	 * * : JSON text file
	 *
	 * @param filename - with extension
	 * @return PersistVistor* - the persist visitor
	 */
	PersistVisitor* create(std::string filename);

};

#endif /* PERSISTENCY_PERSISTVISITORFACTORY_HH_ */
