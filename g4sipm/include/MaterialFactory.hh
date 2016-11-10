/*
 * MaterialFactory.hh
 *
 * @date Mar 15, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef MATERIALFACTORY_HH_
#define MATERIALFACTORY_HH_

#include <G4Material.hh>

/**
 * Factory class (singleton) collecting all used materials. Properly initializes the materials with index of refraction.
 */
class MaterialFactory {
private:
	G4Material* air;
	G4Material* boronCarbideCeramic;
	G4Material* copper;
	G4Material* epoxy;
	G4Material* silicon;

	/**
	 * Hidden constructor.
	 */
	MaterialFactory();

public:
	static double LAMBDA_MIN;
	static double LAMBDA_MAX;

	virtual ~MaterialFactory();

	/**
	 * @return MaterialFactory - the singleton.
	 */
	static MaterialFactory* getInstance();

	/**
	 * @return G4Material - air.
	 */
	G4Material* getAir();

	/**
	 * @return G4Material - ceramic.
	 */
	G4Material* getBoronCarbideCeramic();

	/**
	 * @return G4Material - copper.
	 */
	G4Material* getCopper();

	/**
	 * @return G4Material - epoxy.
	 */
	G4Material* getEpoxy();

	/**
	 * @return G4Material - silicon.
	 */
	G4Material* getSilicon();
};

#endif /* MATERIALFACTORY_HH_ */
