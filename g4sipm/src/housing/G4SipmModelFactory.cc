/*
 * G4SipmModelFactory.cc
 *
 * @created Apr 29, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "model/G4SipmModelFactory.hh"

#include "model/impl/G4SipmGenericSipmModel.hh"
#include "model/impl/HamamatsuS1036211100.hh"
#include "model/impl/HamamatsuS1036233100.hh"
#include "model/impl/HamamatsuS1036233050.hh"
#include "model/impl/HamamatsuS10985100.hh"
#include "model/impl/HamamatsuS12651050.hh"
#include "model/impl/HamamatsuS12573100C.hh"
#include "model/impl/HamamatsuS12573100X.hh"
#include "model/impl/G4SipmConfigFileModel.hh"

G4SipmModelFactory::G4SipmModelFactory() {
	//
}

G4SipmModelFactory* G4SipmModelFactory::getInstance() {
	static G4SipmModelFactory* instance = new G4SipmModelFactory;
	return instance;
}

G4SipmModel* G4SipmModelFactory::createConfigFileModel(std::string propertiesFileName) const {
	return new G4SipmConfigFileModel(propertiesFileName);
}

G4SipmModel* G4SipmModelFactory::createGenericSipmModel() const {
	return new G4SipmGenericSipmModel;
}

G4SipmModel* G4SipmModelFactory::createHamamatsuS1036211100() const {
	return new HamamatsuS1036211100;
}

G4SipmModel* G4SipmModelFactory::createHamamatsuS1036233100() const {
	return new HamamatsuS1036233100;
}

G4SipmModel* G4SipmModelFactory::createHamamatsuS1036233050() const {
	return new HamamatsuS1036233050;
}

G4SipmModel* G4SipmModelFactory::createHamamatsuS10985100() const {
	return new HamamatsuS10985100;
}

G4SipmModel* G4SipmModelFactory::createHamamatsuS12651050() const {
	return new HamamatsuS12651050;
}

G4SipmModel* G4SipmModelFactory::createHamamatsuS12573100C() const {
	return new HamamatsuS12573100C;
}

G4SipmModel* G4SipmModelFactory::createHamamatsuS12573100X() const {
	return new HamamatsuS12573100X;
}
