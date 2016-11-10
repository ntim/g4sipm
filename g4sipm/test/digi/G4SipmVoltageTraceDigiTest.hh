/*
 * G4SipmTraceDigiTest.hh
 *
 * @date 20.08.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMTRACEDIGITEST_HH_
#define G4SIPMTRACEDIGITEST_HH_

#include <gtest/gtest.h>

#include "digi/G4SipmVoltageTraceDigi.hh"
#include "model/impl/G4SipmGenericVoltageTraceModel.hh"

class G4SipmVoltageTraceDigiTest: public testing::Test {
protected:
	double tMin;
	double tMax;
	G4SipmVoltageTraceDigi* testedObject;

	virtual void SetUp() {
		tMin = 0;
		tMax = 1000 * CLHEP::ns;
		testedObject = new G4SipmVoltageTraceDigi(0, tMin, tMax, 1. * CLHEP::ns);
	}
};

TEST_F(G4SipmVoltageTraceDigiTest, shouldInitialize) {
	ASSERT_EQ((size_t )1000, testedObject->getContainer().size());
}

TEST_F(G4SipmVoltageTraceDigiTest, index) {
	ASSERT_EQ((size_t) 0, testedObject->index(tMin));
	ASSERT_EQ((size_t) 500, testedObject->index(tMin + (tMax - tMin) / 2.));
}

TEST_F(G4SipmVoltageTraceDigiTest, time) {
	ASSERT_EQ(tMin + (tMax - tMin) / 2., testedObject->time(500));
}

TEST_F(G4SipmVoltageTraceDigiTest, at) {
	for (size_t i = 0; i < testedObject->size(); i++) {
		testedObject->at(i);
	}
	for (double t = tMin; t < tMax; t += 1.0 * CLHEP::ns) {
		testedObject->at(testedObject->index(t));
	}
}

TEST_F(G4SipmVoltageTraceDigiTest, atMeasured) {
	// Add a pulse to the trace.
	G4SipmVoltageTraceModel* model = new G4SipmGenericVoltageTraceModel;
	for (double t = tMin; t < tMax; t += 1.0 * CLHEP::ns) {
		testedObject->at(testedObject->index(t)) += model->pulse(t - tMin);
	}
	// Traverse the trace again and retreive the measured value.
	const double factor = pow(2, testedObject->getPrecision()) / testedObject->getVoltageRange();
	for (double t = tMin; t < tMax; t += 1.0 * CLHEP::ns) {
		ASSERT_EQ(round(model->pulse(t - tMin) * factor) / factor, testedObject->atMeasured(testedObject->index(t)));
	}
}

#endif /* G4SIPMTRACEDIGITEST_HH_ */
