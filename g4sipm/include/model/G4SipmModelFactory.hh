/*
 * G4SipmModelFactory.hh
 *
 * @created Apr 29, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMMODELFACTORY_HH_
#define G4SIPMMODELFACTORY_HH_

#include <string>

#include "model/G4SipmModel.hh"

/**
 * Factory class for simple SiPM model creation.
 */
class G4SipmModelFactory {
private:
	G4SipmModelFactory();

public:
	/**
	 * @return G4SipmModelFactory - the singleton instance.
	 */
	static G4SipmModelFactory* getInstance();

	/**
	 * @param propertiesFileName - the path to the properties file.
	 * @return G4SipmModel - the SiPM config file model.
	 */
	G4SipmModel* createConfigFileModel(std::string propertiesFileName) const;
	/**
	 * @return G4SipmModel - the generic SiPM model.
	 */
	G4SipmModel* createGenericSipmModel() const;
	/**
	 * @return G4SipmModel - the HamamatsuS1036211100 SiPM model.
	 */
	G4SipmModel* createHamamatsuS1036211100() const;
	/**
	 * @return G4SipmModel - the HamamatsuS1036233100 SiPM model.
	 */
	G4SipmModel* createHamamatsuS1036233100() const;
	/**
	 * @return G4SipmModel - the HamamatsuS1036233050 SiPM model.
	 */
	G4SipmModel* createHamamatsuS1036233050() const;
	/**
	 * @return G4SipmModel - the HamamatsuS10985100 SiPM model.
	 */
	G4SipmModel* createHamamatsuS10985100() const;
	/**
	 * @return G4SipmModel - the HamamatsuS12651050 SiPM model.
	 */
	G4SipmModel* createHamamatsuS12651050() const;
	/**
	 * @return G4SipmModel - the HamamatsuS12573100C SiPM model.
	 */
	G4SipmModel* createHamamatsuS12573100C() const;
	/**
	 * @return G4SipmModel - the HamamatsuS12573100X SiPM model.
	 */
	G4SipmModel* createHamamatsuS12573100X() const;
};

#endif /* G4SIPMMODELFACTORY_HH_ */
