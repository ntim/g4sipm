/*
 * G4SipmRandomGainMapModelTest.hh
 *
 * @created Apr 25, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMRANDOMGAINMAPMODELTEST_HH_
#define G4SIPMRANDOMGAINMAPMODELTEST_HH_

#include <gtest/gtest.h>

#include "model/impl/G4SipmGenericSipmModel.hh"
#include "model/impl/G4SipmRandomGainMapModel.hh"

class G4SipmRandomGainMapModelTest: public testing::Test {
protected:

	class SipmModel: public G4SipmGenericSipmModel {
		virtual unsigned int getNumberOfCells() const {
			return 14400;
		}
	};

	G4SipmGainMapModel* testedObject;
	G4SipmModel* sipmModel;

	virtual void SetUp() {
		sipmModel = new SipmModel;
		testedObject = new G4SipmRandomGainMapModel;
		testedObject->refresh(sipmModel);
	}
};

TEST_F(G4SipmRandomGainMapModelTest, refresh) {
	testedObject->refresh(new SipmModel);
}

TEST_F(G4SipmRandomGainMapModelTest, needsRefresh) {
	ASSERT_FALSE(testedObject->needsRefresh());
}

TEST_F(G4SipmRandomGainMapModelTest, getGain) {
	double nCells = 1. * sipmModel->getNumberOfCells();
	// Compute mean.
	double mean = 0.0;
	for (G4SipmCellId id = 0; id < nCells; id++) {
		mean += testedObject->getGain(id) / nCells;
	}
	ASSERT_NEAR(1.0, mean, 0.01);
	// Compute variance.
	double sigma = 0.0;
	for (G4SipmCellId id = 0; id < nCells; id++) {
		double v = testedObject->getGain(id);
		sigma += (v - mean) * (v - mean) / (nCells - 1);
	}
	sigma = sqrt(sigma);
	ASSERT_NEAR(sipmModel->getGainVariation(), sigma, 0.02 * sipmModel->getGainVariation());
}

#endif /* G4SIPMRANDOMGAINMAPMODELTEST_HH_ */
