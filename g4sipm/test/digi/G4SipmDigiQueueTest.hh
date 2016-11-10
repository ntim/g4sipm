/*
 * G4SipmDigiSortedQueueTest.hh
 *
 * @date 26.02.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMDIGISORTEDQUEUETEST_HH_
#define G4SIPMDIGISORTEDQUEUETEST_HH_

#include <gtest/gtest.h>
#include <vector>
#include <Randomize.hh>

#include "digi/G4SipmDigi.hh"
#include "digi/G4SipmDigiQueue.hh"

class G4SipmDigiQueueTest: public testing::Test {
protected:
	G4SipmDigiQueue testedObject;
	std::vector<G4SipmDigi*> digis;

	virtual void SetUp() {
		const unsigned int nDigis = 10000;
		for (unsigned int i = 0; i < nDigis; ++i) {
			G4SipmDigi* digi = new G4SipmDigi;
			digi->setTime(G4UniformRand());
			digis.push_back(digi);
		}
		testedObject.pushAll(digis);
	}
};

TEST_F(G4SipmDigiQueueTest, size) {
	ASSERT_EQ(digis.size(), testedObject.size());
}

TEST_F(G4SipmDigiQueueTest, order) {
	G4SipmDigi* old = testedObject.next();
	unsigned int number = 1;
	while (testedObject.hasNext()) {
		G4SipmDigi* next = testedObject.next();
		ASSERT_TRUE(old->getTime() <= next->getTime());
		old = next;
		number++;
	}
	ASSERT_EQ(digis.size(), number);
}

TEST_F(G4SipmDigiQueueTest, shouldNotAddNull) {
	testedObject.push(NULL);
	ASSERT_EQ(digis.size(), testedObject.size());
}

#endif /* G4SIPMDIGISORTEDQUEUETEST_HH_ */
