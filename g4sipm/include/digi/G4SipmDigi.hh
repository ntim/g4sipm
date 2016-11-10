/*
 * G4SipmDigi.hh
 *
 * @created Apr 11, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4SIPMDIGI_HH_
#define G4SIPMDIGI_HH_

#include <G4VDigi.hh>
#include <G4Allocator.hh>
#include <G4TDigiCollection.hh>

#include "G4Sipm.hh"

/**
 * Enumeration signifying the cause of the cell trigger of the Sipm.
 */
enum G4SipmDigiType {
	UNDEFINED, PHOTON, THERMAL, CROSSTALK, AFTERPULSE
};

/**
 * Geant4 Digi for the G4Sipm. Signifies one cell trigger.
 */
class G4SipmDigi: public G4VDigi {
private:
	G4SipmId sipmId;
	double time;
	G4SipmCellId cellId;
	double weight;
	G4SipmDigiType type;

public:
	/**
	 * Constructor.
	 */
	G4SipmDigi();
	/**
	 * Copy constructor.
	 */
	G4SipmDigi(const G4SipmDigi& right);
	virtual ~G4SipmDigi();

	const G4SipmDigi& operator=(const G4SipmDigi&);
	int operator==(const G4SipmDigi&) const;
	inline void* operator new(size_t);
	inline void operator delete(void* aDigi);

	/**
	 * @return G4SipmId - the SiPM id.
	 */
	G4SipmId getSipmId() const;
	/**
	 * @param sipmId - the SiPM id to set.
	 */
	void setSipmId(G4SipmId sipmId);
	/**
	 * @return G4SipmCellId - the cell id.
	 */
	G4SipmCellId getCellId() const;
	/**
	 * @param cellId - the cellId to set.
	 */
	void setCellId(G4SipmCellId cellId);
	/**
	 * @return double - the global time of the trigger.
	 */
	double getTime() const;
	/**
	 * @param time - the time to set.
	 */
	void setTime(double time);
	/**
	 * @return G4SipmDigiType - the type of the trigger.
	 */
	G4SipmDigiType getType() const;
	/**
	 * @param type - the type of the trigger.
	 */
	void setType(G4SipmDigiType type);
	/**
	 * @return double - the weight of the trigger which is identical to the gain of the cell.
	 */
	double getWeight() const;
	/**
	 * @param weight - the weight of the trigger.
	 */
	void setWeight(double weight);
};

typedef G4TDigiCollection<G4SipmDigi> G4SipmDigiCollection;

extern G4ThreadLocal G4Allocator<G4SipmDigi>* G4SipmDigiAllocator;

inline void* G4SipmDigi::operator new(size_t) {
	if(!G4SipmDigiAllocator) {
		G4SipmDigiAllocator = new G4Allocator<G4SipmDigi>;
	}
	return (void*) G4SipmDigiAllocator->MallocSingle();
}

inline void G4SipmDigi::operator delete(void* aDigi) {
	G4SipmDigiAllocator->FreeSingle((G4SipmDigi*) aDigi);
}

#endif /* G4SIPMDIGI_HH_ */
