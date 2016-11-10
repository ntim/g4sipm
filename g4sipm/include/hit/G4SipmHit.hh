/*
 * G4SipmHit.hh
 *
 * @date Mar 13, 2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMHIT_HH_
#define G4SIPMHIT_HH_

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4ParticleDefinition.hh>
#include <CLHEP/Vector/ThreeVector.h>

#include "G4Sipm.hh"

/**
 * Hit class for the G4Sipm. It implements G4VHit#Draw() to visualize hits as red
 * dots on the SiPM surface.
 */
class G4SipmHit: public G4VHit {
private:
	G4SipmId sipmId;
	int pdgId;
	int trackId;
	double eKin;
	double time;
	double weight;
	CLHEP::Hep3Vector position;
	CLHEP::Hep3Vector worldPosition;
	CLHEP::Hep3Vector momentum;
	CLHEP::Hep3Vector startPosition;
	CLHEP::Hep3Vector startMomentum;

public:
	/**
	 * Constructor.
	 */
	G4SipmHit();
	virtual ~G4SipmHit();

	virtual void Draw();
	inline void* operator new(size_t);
	inline void operator delete(void*);

	/**
	 * @return G4ParticleDefinition - the particle definition for the underlying PDG-Id.
	 */
	G4ParticleDefinition* getParticleDefinition() const;

	/**
	 * @return double - the kinetic energy.
	 */
	double getEKin() const;
	/**
	 * @param eKin - the kinetic energy to set.
	 */
	void setEKin(double eKin);
	/**
	 * @return Hep3Vector - the momentum.
	 */
	const CLHEP::Hep3Vector& getMomentum() const;
	/**
	 * @param momentum - the momentum to set.
	 */
	void setMomentum(const CLHEP::Hep3Vector& momentum);
	/**
	 * @return int - the PDG particle id.
	 */
	int getPdgId() const;
	/**
	 * @param pdgId - the PDG particle id to set.
	 */
	void setPdgId(int pdgId);
	/**
	 * @return Hep3Vector - the position.
	 */
	const CLHEP::Hep3Vector& getPosition() const;
	/**
	 * @param position - the position to set.
	 */
	void setPosition(const CLHEP::Hep3Vector& position);
	/**
	 * @return G4SipmId - the SiPM id to set.
	 */
	G4SipmId getSipmId() const;
	/**
	 * @param sipmId - the SiPM id to set.
	 */
	void setSipmId(G4SipmId sipmId);
	/**
	 * @return int - the track id.
	 */
	int getTrackId() const;
	/**
	 * @param trackId - the track id.
	 */
	void setTrackId(int trackId);
	/**
	 * @return Hep3Vector - the start momentum.
	 */
	const CLHEP::Hep3Vector& getStartMomentum() const;
	/**
	 * @param startMomentum - the start momentum to set.
	 */
	void setStartMomentum(const CLHEP::Hep3Vector& startMomentum);
	/**
	 * @return Hep3Vector - the start position.
	 */
	const CLHEP::Hep3Vector& getStartPosition() const;
	/**
	 * @param startPosition - the start position to set.
	 */
	void setStartPosition(const CLHEP::Hep3Vector& startPosition);
	/**
	 * @return double - the time.
	 */
	double getTime() const;
	/**
	 * @param time - the time to set.
	 */
	void setTime(double time);
	/**
	 * @return double - the weight of the particle step.
	 */
	double getWeight() const;
	/**
	 * @param weight - the weight to set.
	 */
	void setWeight(double weight);
	/**
	 * @return Hep3Vector - the position relative to the origin of the world.
	 */
	const CLHEP::Hep3Vector& getWorldPosition() const;
	/**
	 * @param worldPosition - the position relative to the origin of the world.
	 */
	void setWorldPosition(const CLHEP::Hep3Vector& worldPosition);
};

typedef G4THitsCollection<G4SipmHit> G4SipmHitsCollection;

extern G4ThreadLocal G4Allocator<G4SipmHit>* G4SipmHitAllocator;

inline void* G4SipmHit::operator new(size_t) {
	if (!G4SipmHitAllocator) {
		G4SipmHitAllocator = new G4Allocator<G4SipmHit>;
	}
	return (void*) G4SipmHitAllocator->MallocSingle();
}

inline void G4SipmHit::operator delete(void* hit) {
	G4SipmHitAllocator->FreeSingle((G4SipmHit*) hit);
}

#endif /* G4SIPMHIT_HH_ */
