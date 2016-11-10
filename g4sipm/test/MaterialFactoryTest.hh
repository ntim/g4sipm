/*
 * MaterialFactoryTest.hh
 *
 * @date 18.08.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef MATERIALFACTORYTEST_HH_
#define MATERIALFACTORYTEST_HH_

#include <gtest/gtest.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <G4MaterialPropertiesTable.hh>
#include <G4MaterialPropertyVector.hh>

#include "MaterialFactory.hh"

class MaterialFactoryTest: public testing::Test {
protected:
	/**
	 * Test energy.
	 */
	double testEn;

	virtual void SetUp() {
		double testWvl = 350.0 * CLHEP::nanometer;
		testEn = CLHEP::h_Planck * CLHEP::c_light / testWvl;
	}
};

TEST_F(MaterialFactoryTest, getAir) {
	G4Material* air = MaterialFactory::getInstance()->getAir();
	ASSERT_TRUE(air != NULL);
	ASSERT_NEAR(1., air->GetMaterialPropertiesTable()->GetProperty("RINDEX")->Value(testEn), 1e-3);
}

TEST_F(MaterialFactoryTest, getBoronCarbideCeramic) {
	G4Material* ceramic = MaterialFactory::getInstance()->getBoronCarbideCeramic();
	ASSERT_TRUE(ceramic != NULL);
}

TEST_F(MaterialFactoryTest, getCopper) {
	G4Material* copper = MaterialFactory::getInstance()->getCopper();
	ASSERT_TRUE(copper != NULL);
}

TEST_F(MaterialFactoryTest, getEpoxy) {
	G4Material* epoxy = MaterialFactory::getInstance()->getEpoxy();
	ASSERT_TRUE(epoxy != NULL);
	ASSERT_NEAR(1.5, epoxy->GetMaterialPropertiesTable()->GetProperty("RINDEX")->Value(testEn), 1e-3);
}

TEST_F(MaterialFactoryTest, getSilicon) {
	G4Material* silicon = MaterialFactory::getInstance()->getSilicon();
	ASSERT_TRUE(silicon != NULL);
	// Calculated abs(5.43579 + i * 2.9885).
	ASSERT_NEAR(6.20313994, silicon->GetMaterialPropertiesTable()->GetProperty("RINDEX")->Value(testEn), 1e-3);
	// ASSERT_NEAR(1.0730e6 / cm, silicon->GetMaterialPropertiesTable()->GetProperty("ABSLENGTH")->Value(testEn),
	//		1e6/cm * 1e-3);
}

#endif /* MATERIALFACTORYTEST_HH_ */
