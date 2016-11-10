/*
 * G4SipmGaussianGainMapModelTest.hh
 *
 * @created Apr 30, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMGAUSSIANGAINMAPMODELTEST_HH_
#define G4SIPMGAUSSIANGAINMAPMODELTEST_HH_

#include <gtest/gtest.h>

#include "model/impl/G4SipmGenericSipmModel.hh"
#include "model/impl/G4SipmGaussianGainMapModel.hh"

class G4SipmGaussianGainMapModelTest: public testing::Test {
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
		testedObject = new G4SipmGaussianGainMapModel;
		testedObject->refresh(sipmModel);
	}
};

TEST_F(G4SipmGaussianGainMapModelTest, refresh) {
	testedObject->refresh(sipmModel);
}

TEST_F(G4SipmGaussianGainMapModelTest, needsRefresh) {
	ASSERT_FALSE(testedObject->needsRefresh());
}

TEST_F(G4SipmGaussianGainMapModelTest, getGain) {
	double nCells = 1. * sipmModel->getNumberOfCells();
	// Compute mean.
	double mean = 0.;
	for (G4SipmCellId id = 0; id < nCells; id++) {
		mean += testedObject->getGain(id) / nCells;
	}
	ASSERT_NEAR(1., mean, 0.01);
	// TODO: variance test fails.
}

#endif /* G4SIPMGAUSSIANGAINMAPMODELTEST_HH_ */
