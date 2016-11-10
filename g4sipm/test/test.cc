/*
 * test.cc
 *
 * @date 25.02.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include <gtest/gtest.h>

#include "G4SipmUiMessenger.hh"

#include "G4SipmTest.hh"
#include "G4SipmUiMessengerTest.hh"
#include "InterpolationUtilTest.hh"
#include "MaterialFactoryTest.hh"
#include "PropertiesTest.hh"
#include "VectorUtilTest.hh"
#include "FresnelUtilTest.hh"

#include "hit/G4SipmSensitiveDetectorTest.hh"
#include "hit/G4SipmSensitiveDetectorFilterTest.hh"

#include "digi/G4SipmCellFireControllerTest.hh"
#include "digi/G4SipmDigiQueueTest.hh"
#include "digi/G4SipmDigitizerTest.hh"
#include "digi/G4SipmVoltageTraceDigiTest.hh"
#include "digi/G4SipmVoltageTraceDigitizerTest.hh"

#include "housing/G4SipmHousingTest.hh"
#include "housing/impl/HamamatsuCeramicHousingTest.hh"
#include "housing/impl/HamamatsuSmdHousingTest.hh"

#include "model/G4SipmModelTest.hh"
#include "model/G4SipmVoltageTraceModelTest.hh"
#include "model/impl/HamamatsuS1036211100Test.hh"
#include "model/impl/HamamatsuS1036233100Test.hh"
#include "model/impl/HamamatsuS10985100Test.hh"
#include "model/impl/G4SipmRandomGainMapModelTest.hh"
#include "model/impl/G4SipmGaussianGainMapModelTest.hh"
#include "model/impl/G4SipmConfigFileModelTest.hh"
#include "model/impl/G4SipmConfigFileVoltageTraceModelTest.hh"

#include "sim/TestRun.hh"

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	// Reset UI messenger to default values.
	G4SipmUiMessenger::getInstance()->setDefaults();
	// Run all tests.
	return RUN_ALL_TESTS();
}
