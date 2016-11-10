/*
 * G4SipmCellFireControllerTest.hh
 *
 * @date 26.02.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMCELLFIRECONTROLLERTEST_HH_
#define G4SIPMCELLFIRECONTROLLERTEST_HH_

#include <gtest/gtest.h>
#include <vector>
#include <math.h>
#include <CLHEP/Random/RandFlat.h>
#include <CLHEP/Random/RandGauss.h>

#include "digi/G4SipmCellFireController.hh"
#include "digi/G4SipmDigiQueue.hh"
#include "model/impl/G4SipmGenericSipmModel.hh"

class G4SipmCellFireControllerTest: public testing::Test {
protected:
	G4SipmModel* model;
	G4SipmCellFireController* testedObject;
	std::vector<G4SipmDigi*> digis;

	virtual void SetUp() {
		model = new G4SipmGenericSipmModel;
		testedObject = new G4SipmCellFireController(model, 0.);
		const unsigned int nDigis = 10000;
		for (unsigned int i = 0; i < nDigis; ++i) {
			G4SipmDigi* digi = new G4SipmDigi;
			digi->setCellId(CLHEP::RandFlat::shootInt(100));
			digi->setTime(0);
			digis.push_back(digi);
		}
	}
};

TEST_F(G4SipmCellFireControllerTest, fireSimple) {
	std::set<int> firedCells;
	for (unsigned int i = 0; i < digis.size(); ++i) {
		G4SipmDigi* digi = digis[i];
		if (testedObject->fire(digi)) {
			if (firedCells.count(digi->getCellId()) == 0) {
				firedCells.insert(digi->getCellId());
			} else {
				FAIL();
			}
		}
	}
}

TEST_F(G4SipmCellFireControllerTest, fire) {
	G4SipmDigi* digi = new G4SipmDigi;
	digi->setSipmId(0);
	digi->setCellId(0);
	// Test firing with time difference larger than the deadTime.
	digi->setTime(model->getRecoveryTime());
	digi->setCellId(1);
	ASSERT_TRUE(testedObject->fire(digi));
	// Test firing with time difference equal to the deadTime.
	digi->setTime(model->getDeadTime());
	digi->setCellId(2);
	ASSERT_FALSE(testedObject->fire(digi));
}

#endif /* G4SIPMCELLFIRECONTROLLERTEST_HH_ */
