/*
 * test.cc
 *
 * @date 25.02.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <gtest/gtest.h>

#include "persistency/PersistVisitorFactoryTest.hh"

#include "rnd/DirectionTest.hh"
#include "rnd/FixedValueTest.hh"
#include "rnd/GeneralTest.hh"
#include "rnd/ThreeVectorTest.hh"

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	// Run all tests.
	return RUN_ALL_TESTS();
}
