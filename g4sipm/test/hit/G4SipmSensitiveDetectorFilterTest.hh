/*
 * G4SipmSensitiveDetectorFilterTest.hh
 *
 * @date 25.02.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMSENSITIVEDETECTORFILTERTEST_HH_
#define G4SIPMSENSITIVEDETECTORFILTERTEST_HH_

#include <gtest/gtest.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4SDManager.hh>
#include <G4HCtable.hh>
#include <G4Step.hh>
#include <G4StepPoint.hh>
#include <G4TouchableHistory.hh>
#include <G4TouchableHandle.hh>
#include <G4DynamicParticle.hh>
#include <G4OpticalPhoton.hh>
#include <G4Electron.hh>

#include "G4Sipm.hh"
#include "G4SipmUiMessenger.hh"
#include "hit/G4SipmSensitiveDetectorFilter.hh"
#include "sim/TestRun.hh"
#include "FresnelUtil.hh"

class G4SipmSensitiveDetectorFilterTest: public testing::Test {
protected:
	TestRun* testRun;
	G4Sipm* sipm;
	G4SipmSensitiveDetectorFilter* testedObject;

	virtual void SetUp() {
		testRun = new TestRun;
		sipm = testRun->getSipm();
		testedObject = new G4SipmSensitiveDetectorFilter(sipm);
	}

	virtual void TearDown() {
		delete testRun;
	}

	G4Step* createStep(G4ThreeVector position = G4ThreeVector(0, 0, 0), G4double wavelength = 350. * CLHEP::nm,
			G4double angle = 0, G4ParticleDefinition * particleDef = G4OpticalPhoton::Definition()) {
		G4Step* step = new G4Step();
		// Create pre-step point.
		G4StepPoint* point = new G4StepPoint();
		point->SetPosition(position);
		point->SetKineticEnergy(CLHEP::h_Planck * CLHEP::c_light / wavelength);
		G4ThreeVector dir;
		dir.setRThetaPhi(1, angle, 0);
		point->SetMomentumDirection(dir);
		// Create touchable.
		G4TouchableHistory* touchable = new G4TouchableHistory;
		G4TouchableHandle handle(touchable);
		point->SetTouchableHandle(handle);
		//
		step->SetPreStepPoint(point);
		// Create track.
		G4DynamicParticle * particle = new G4DynamicParticle(particleDef, point->GetKineticEnergy(),
				dir * point->GetKineticEnergy());
		step->SetTrack(new G4Track(particle, 0, position));
		return step;
	}

	double getRindex(G4Material* m, G4double wavelength = 350. * CLHEP::nm) {
		G4double eKin = CLHEP::h_Planck * CLHEP::c_light / wavelength;
		G4MaterialPropertyVector* property = m->GetMaterialPropertiesTable()->GetProperty("RINDEX");
		return property->Value(eKin);
	}
};

TEST_F(G4SipmSensitiveDetectorFilterTest, shouldStopAndKill) {
	G4Step* step = createStep(G4ThreeVector(50 * CLHEP::micrometer, 50 * CLHEP::micrometer));
	while (!testedObject->Accept(step)) {
		;
	}
	ASSERT_EQ(fStopAndKill, step->GetTrack()->GetTrackStatus());
}

TEST_F(G4SipmSensitiveDetectorFilterTest, shouldRejectOtherThanPhoton) {
	G4Step* step = createStep(G4ThreeVector(0, 0, 0), 350. * CLHEP::nm, 0, G4Electron::Definition());
	ASSERT_FALSE(testedObject->Accept(step));
}

TEST_F(G4SipmSensitiveDetectorFilterTest, shouldRejectOutside) {
	G4Step* step = createStep(G4ThreeVector(-1. * CLHEP::m, 0, 0));
	ASSERT_FALSE(testedObject->Accept(step));
}

TEST_F(G4SipmSensitiveDetectorFilterTest, shouldAcceptWithPde) {
	G4ThreeVector pos = sipm->getModel()->getCellPosition(0);
	G4Step* step = createStep(pos, 350. * CLHEP::nm);
	const int nIter = 100000;
	double nAccept = 0;
	for (int i = 0; i < nIter; ++i) {
		if (testedObject->Accept(step)) {
			nAccept++;
		}
	}
	double n1 = getRindex(MaterialFactory::getInstance()->getAir());
	double n2 = getRindex(sipm->getModel()->getWindowMaterial());
	double n3 = getRindex(sipm->getModel()->getMaterial());
	double t = FresnelUtil::tP(n1, n2, n3, 0.);
	double pde = sipm->getModel()->getPhotonDetectionEfficiency(350. * CLHEP::nm) / sipm->getModel()->getFillFactor()
			/ t;
	EXPECT_NEAR(pde, nAccept / nIter, 0.01);
}

TEST_F(G4SipmSensitiveDetectorFilterTest, shouldAcceptWithGeometry) {
	// Disable PDE check.
	G4UImanager::GetUIpointer()->ApplyCommand("/g4sipm/filter/pde false");
	//
	const int nIter = 10000;
	double nAccept = 0;
	for (int i = 0; i < nIter; ++i) {
		double x = (G4UniformRand()- 0.5) * sipm->getModel()->getPitch();
		double y = (G4UniformRand() - 0.5) * sipm->getModel()->getPitch();
		G4ThreeVector pos(x, y, 0);
		G4Step* step = createStep(pos, 350. * CLHEP::nm);
		if (testedObject->Accept(step)) {
			nAccept++;
		}
	}
	EXPECT_NEAR(sipm->getModel()->getFillFactor(), nAccept / nIter, 0.01);
}

#endif /* G4SIPMSENSITIVEDETECTORFILTERTEST_HH_ */
