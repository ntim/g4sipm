/*
 * ParticleSourceMessenger.hh
 *
 * @date Jan 18, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef PARTICLESOURCEMESSENGER_HH_
#define PARTICLESOURCEMESSENGER_HH_

#include <G4UImessenger.hh>
#include <G4UIcommand.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include "G4UIcmdWithADouble.hh"
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4UIcmdWith3Vector.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithABool.hh>

/**
 * A messenger for configuring the GeneralParticleSource.
 *
 * Commands:
 * /ps/verbose
 * /ps/nParticles
 * /ps/polar
 * /ps/tMin
 * /ps/tMax
 * /ps/time/input
 * /ps/energy/eMin
 * /ps/energy/eMax
 * /ps/energy/input
 * /ps/angle/phiMin
 * /ps/angle/phiMax
 * /ps/angle/thetaMin
 * /ps/angle/thetaMax
 * /ps/plane/surfaceNormal
 * /ps/plane/a
 * /ps/plane/b
 * /ps/plane/pos
 */
class ParticleSourceMessenger: public G4UImessenger {
	friend std::ostream& operator <<(std::ostream& o, const ParticleSourceMessenger& m);

private:
	// Plain values
	int verbose;
	int nParticles;
	double polar;
	double tMin;
	double tMax;
	G4String tInput;
	double eMin;
	double eMax;
	G4String eInput;
	double phiMin;
	double phiMax;
	double thetaMin;
	double thetaMax;
	G4ThreeVector surfaceNormal;
	double a;
	double b;
	G4ThreeVector pos;

	// Commands
	G4UIcmdWithAnInteger* verboseCmd;
	G4UIcmdWithAnInteger* nParticlesCmd;
	G4UIcmdWithADoubleAndUnit* tMinCmd;
	G4UIcmdWithADoubleAndUnit* tMaxCmd;
	G4UIcmdWithAString* tInputCmd;
	G4UIcmdWithADoubleAndUnit* polarCmd;
	G4UIcmdWithADoubleAndUnit* eMinCmd;
	G4UIcmdWithADoubleAndUnit* eMaxCmd;
	G4UIcmdWithAString* eInputCmd;
	G4UIcmdWithADoubleAndUnit* phiMinCmd;
	G4UIcmdWithADoubleAndUnit* phiMaxCmd;
	G4UIcmdWithADoubleAndUnit* thetaMinCmd;
	G4UIcmdWithADoubleAndUnit* thetaMaxCmd;
	G4UIcmdWith3VectorAndUnit* surfaceNormalCmd;
	G4UIcmdWithADoubleAndUnit* aCmd;
	G4UIcmdWithADoubleAndUnit* bCmd;
	G4UIcmdWith3VectorAndUnit* posCmd;

	/**
	 * Hidden constructor.
	 */
	ParticleSourceMessenger();

public:
	/**
	 * The file name of the macro file.
	 */
	static const std::string MACRO_FILE_NAME;
	/**
	 * @return GeneralParticleSourceMessenger - the singleton.
	 */
	static ParticleSourceMessenger* getInstance();

	virtual ~ParticleSourceMessenger();

	void SetNewValue(G4UIcommand*, G4String);
	void setDefaultValues();

	/**
	 * Parses the program options and sets the messenger parameters accordingly.
	 *
	 * @param argc - the argument count.
	 * @param argv - the arguments.
	 */
	void parseProgramOptions(int argc, char** argv);

	/**
	 * @return int - the verbosity.
	 */
	int getVerbose() const;
	/**
	 * @param verbose - the verbosity to set.
	 */
	void setVerbose(int verbose);
	/**
	 * @return int - the number of particles per beamOn.
	 */
	int getNParticles() const;
	/**
	 * @param nParticles - the nParticles to set.
	 */
	void setNParticles(int nParticles);
	/**
	 * @return double - the polar angle of photons.
	 */
	double getPolar() const;
	/**
	 * @param polar - the polar to set.
	 */
	void setPolar(double polar);
	/**
	 * @return double - the eMax.
	 */
	double getEMax() const;
	/**
	 * @param eMax - the eMax to set.
	 */
	void setEMax(double eMax);
	/**
	 * @return double - the eMin.
	 */
	double getEMin() const;
	/**
	 * @param eMin - the eMin to set.
	 */
	void setEMin(double eMin);
	/**
	 * @return double - the phiMax.
	 */
	double getPhiMax() const;
	/**
	 * @param phiMax - the phiMax to set.
	 */
	void setPhiMax(double phiMax);
	/**
	 * @return double - the phiMin.
	 */
	double getPhiMin() const;
	/**
	 * @param phiMin - the phiMin to set.
	 */
	void setPhiMin(double phiMin);
	/**
	 * @return double - the thetaMax.
	 */
	double getThetaMax() const;
	/**
	 * @param thetaMax - the thetaMax to set.
	 */
	void setThetaMax(double thetaMax);
	/**
	 * @return double - the thetaMin.
	 */
	double getThetaMin() const;
	/**
	 * @param thetaMin - the thetaMin to set.
	 */
	void setThetaMin(double thetaMin);
	/**
	 * @return G4ThreeVector - the position of the particle plane.
	 */
	G4ThreeVector getPos() const;
	/**
	 * @param pos - the pos to set.
	 */
	void setPos(G4ThreeVector pos);
	/**
	 * @return G4ThreeVector - the surface normal.
	 */
	G4ThreeVector getSurfaceNormal() const;
	/**
	 * @param surfaceNormal - the surfaceNormal to set.
	 */
	void setSurfaceNormal(G4ThreeVector surfaceNormal);
	/**
	 * @return double - the length of the rectangular source plane.
	 */
	double getA() const;
	/**
	 * @param a - the a to set.
	 */
	void setA(double a);
	/**
	 * @return double - the width of the rectangular source plane.
	 */
	double getB() const;
	/**
	 * @param b - the b to set.
	 */
	void setB(double b);
	/**
	 * @return double - the minimum particle time.
	 */
	double getTMin() const;
	/**
	 * @param tMin - the tMin to set.
	 */
	void setTMin(double tMin);
	/**
	 * @return double - the maximum particle time.
	 */
	double getTMax() const;
	/**
	 * @param tMax - the tMax to set.
	 */
	void setTMax(double tMax);
	/**
	 * @return G4String - the time probability distribution function file.
	 */
	G4String getTInput() const;
	/**
	 * @return G4String - the energy probability distribution function file.
	 */
	G4String getEInput() const;
};

#endif /* PARTICLESOURCEMESSENGER_HH_ */
