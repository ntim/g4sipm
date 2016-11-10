/*
 * GeneralParticleSourceMessenger.cc
 *
 * @date Jan 18, 2011
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "ParticleSourceMessenger.hh"

#include <boost/program_options.hpp>

#include <G4SystemOfUnits.hh>
#include <G4UImanager.hh>
#include <G4OpticalPhoton.hh>

#include "G4UiMessengerUtil.hh"
#include "ProgramOptionsUtil.hh"

const std::string ParticleSourceMessenger::MACRO_FILE_NAME = "ps.mac";

ParticleSourceMessenger::ParticleSourceMessenger() :
		G4UImessenger() {
	setDefaultValues();
	// Create directories.
	new G4UIdirectory("/ps/");
	new G4UIdirectory("/ps/energy/");
	new G4UIdirectory("/ps/angle/");
	new G4UIdirectory("/ps/plane/");
	// Create basic commands.
	verboseCmd = G4UiMessengerUtil::createCmd(this, "/ps/", "verbose", verbose);
	nParticlesCmd = G4UiMessengerUtil::createCmd(this, "/ps/", "nParticles", nParticles);
	polarCmd = G4UiMessengerUtil::createCmd(this, "/ps/", "polar", polar, "deg");
	// Create time commands.
	tMinCmd = G4UiMessengerUtil::createCmd(this, "/ps/", "tMin", tMin, "s");
	tMaxCmd = G4UiMessengerUtil::createCmd(this, "/ps/", "tMax", tMax, "s");
	tInputCmd = G4UiMessengerUtil::createCmd(this, "/ps/time/", "input", tInput);
	// Create energy commands.
	eMinCmd = G4UiMessengerUtil::createCmd(this, "/ps/energy/", "eMin", eMin, "eV");
	eMaxCmd = G4UiMessengerUtil::createCmd(this, "/ps/energy/", "eMax", eMax, "eV");
	eInputCmd = G4UiMessengerUtil::createCmd(this, "/ps/energy/", "input", eInput);
	// Create angle commands.
	phiMinCmd = G4UiMessengerUtil::createCmd(this, "/ps/angle/", "phiMin", phiMin, "deg");
	phiMaxCmd = G4UiMessengerUtil::createCmd(this, "/ps/angle/", "phiMax", phiMax, "deg");
	thetaMinCmd = G4UiMessengerUtil::createCmd(this, "/ps/angle/", "thetaMin", thetaMin, "deg");
	thetaMaxCmd = G4UiMessengerUtil::createCmd(this, "/ps/angle/", "thetaMax", thetaMax, "deg");
	// Create plane commands.
	surfaceNormalCmd = G4UiMessengerUtil::createCmd(this, "/ps/plane/", "surfaceNormal", surfaceNormal, "mm");
	aCmd = G4UiMessengerUtil::createCmd(this, "/ps/plane/", "a", a, "mm");
	bCmd = G4UiMessengerUtil::createCmd(this, "/ps/plane/", "b", b, "mm");
	posCmd = G4UiMessengerUtil::createCmd(this, "/ps/plane/", "pos", pos, "mm");
	// Set guidance.
	polarCmd->SetGuidance("Set linear polarization angle w.r.t. (k,n) plane. "
			"Negative values will trigger a random polarization.");
	// Set parameter boundaries.
	verboseCmd->SetParameterName("verbose", false);
	verboseCmd->SetRange("verbose >= 0 && verbose <= 2");
	nParticlesCmd->SetParameterName("nParticles", false);
	nParticlesCmd->SetRange("nParticles > 0");
	// Initialize from macro.
	G4UiMessengerUtil::executeMacro(MACRO_FILE_NAME, true);
}

ParticleSourceMessenger::~ParticleSourceMessenger() {
	delete verboseCmd;
	delete nParticlesCmd;
	delete polarCmd;
	delete tMinCmd;
	delete tMaxCmd;
	delete tInputCmd;
	delete eMinCmd;
	delete eMaxCmd;
	delete eInputCmd;
	delete phiMinCmd;
	delete phiMaxCmd;
	delete thetaMinCmd;
	delete thetaMaxCmd;
	delete surfaceNormalCmd;
	delete aCmd;
	delete bCmd;
	delete posCmd;
}

void ParticleSourceMessenger::setDefaultValues() {
	verbose = 0;
	nParticles = 1;
	polar = -360 * deg;
	tMin = 0;
	tMax = 0;
	eMin = 1 * eV;
	eMax = 10 * eV;
	phiMin = 0;
	phiMax = 360 * deg;
	thetaMin = 0;
	thetaMax = 90 * deg;
	surfaceNormal = G4ThreeVector(0., 0., -1.);
	a = 1 * mm;
	b = 1 * mm;
	pos = G4ThreeVector(0, 0, 5 * cm);
}

void ParticleSourceMessenger::SetNewValue(G4UIcommand* cmd, G4String value) {
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, verboseCmd, value, &verbose);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, nParticlesCmd, value, &nParticles);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, polarCmd, value, &polar);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, tMinCmd, value, &tMin);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, tMaxCmd, value, &tMax);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, tInputCmd, value, &tInput);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, eMinCmd, value, &eMin);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, eMaxCmd, value, &eMax);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, eInputCmd, value, &eInput);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, phiMinCmd, value, &phiMin);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, phiMaxCmd, value, &phiMax);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, thetaMinCmd, value, &thetaMin);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, thetaMaxCmd, value, &thetaMax);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, surfaceNormalCmd, value, &surfaceNormal);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, aCmd, value, &a);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, bCmd, value, &b);
	G4UiMessengerUtil::setNewValueIfCmdMatches(cmd, posCmd, value, &pos);
}

template<typename T> void setNewValueIfArgumentGiven(const boost::program_options::variables_map& vm, std::string key,
		T* value) {
	if (vm.count(key)) {
		*value = vm[key].as<T>();
	}
}

void setNewValueIfArgumentGiven(const boost::program_options::variables_map& vm, std::string key, double* value,
		double unit = 1.) {
	if (vm.count(key)) {
		*value = vm[key].as<double>() * unit;
	}
}

void setNewValueIfArgumentGiven(const boost::program_options::variables_map& vm, std::string key, G4String* value) {
	if (vm.count(key)) {
		*value = vm[key].as<std::string>();
	}
}

void ParticleSourceMessenger::parseProgramOptions(int argc, char** argv) {
	// Declare the supported options.
	boost::program_options::options_description programOptions("Particle source options");
	programOptions.add_options()
	// Verbose.
	("verbose", po::value<int>()->default_value(verbose))
	// Number of particles.
	("nParticles", po::value<int>()->default_value(nParticles))
	// Polarization.
	("polar", po::value<double>()->default_value(polar / deg))
	// tMin.
	("tMin", po::value<double>()->default_value(tMin / s))
	// tMax.
	("tMax", po::value<double>()->default_value(tMax / s))
	// Time probability distribution input file.
	("tInput", po::value<std::string>()->default_value(""))
	// eMin.
	("eMin", po::value<double>()->default_value(eMin / eV))
	// eMax.
	("eMax", po::value<double>()->default_value(eMax / eV))
	// Energy probability distribution input file.
	("eInput", po::value<std::string>()->default_value(""))
	// phiMin.
	("phiMin", po::value<double>()->default_value(phiMin / deg))
	// phiMax.
	("phiMax", po::value<double>()->default_value(phiMax / deg))
	// thetaMin.
	("thetaMin", po::value<double>()->default_value(thetaMin / deg))
	// thetaMax.
	("thetaMax", po::value<double>()->default_value(thetaMax / deg))
	// sourceSurfaceNormal.
	("surfaceNormalX", po::value<double>()->default_value(surfaceNormal.x()))("surfaceNormalY",
			po::value<double>()->default_value(surfaceNormal.y()))("surfaceNormalZ",
			po::value<double>()->default_value(surfaceNormal.z()))
	// a.
	("planeA", po::value<double>()->default_value(a / mm))
	// b.
	("planeB", po::value<double>()->default_value(b / mm))
	// Position.
	("posX", po::value<double>()->default_value(pos.x() / mm))("posY", po::value<double>()->default_value(pos.y() / mm))(
			"posZ", po::value<double>()->default_value(pos.z() / mm));
	// Parse program options.
	static po::variables_map vm = ProgramOptionsUtil::parse(argc, argv, programOptions, true);
	// Set new values.
	setNewValueIfArgumentGiven(vm, "verbose", &verbose);
	setNewValueIfArgumentGiven(vm, "nParticles", &nParticles);
	setNewValueIfArgumentGiven(vm, "polar", &polar, deg);
	setNewValueIfArgumentGiven(vm, "tMin", &tMin, s);
	setNewValueIfArgumentGiven(vm, "tMax", &tMax, s);
	setNewValueIfArgumentGiven(vm, "tInput", &tInput);
	setNewValueIfArgumentGiven(vm, "eMin", &eMin, eV);
	setNewValueIfArgumentGiven(vm, "eMax", &eMax, eV);
	setNewValueIfArgumentGiven(vm, "eInput", &eInput);
	setNewValueIfArgumentGiven(vm, "phiMin", &phiMin, deg);
	setNewValueIfArgumentGiven(vm, "phiMax", &phiMax, deg);
	setNewValueIfArgumentGiven(vm, "thetaMin", &thetaMin, deg);
	setNewValueIfArgumentGiven(vm, "thetaMax", &thetaMax, deg);
	if (vm.count("surfaceNormalX") && vm.count("surfaceNormalY") && vm.count("surfaceNormalZ")) {
		double x = vm["surfaceNormalX"].as<double>();
		double y = vm["surfaceNormalY"].as<double>();
		double z = vm["surfaceNormalZ"].as<double>();
		surfaceNormal = G4ThreeVector(x, y, z);
	}
	setNewValueIfArgumentGiven(vm, "planeA", &a, mm);
	setNewValueIfArgumentGiven(vm, "planeB", &b, mm);
	if (vm.count("posX") && vm.count("posY") && vm.count("posZ")) {
		double x = vm["posX"].as<double>() * mm;
		double y = vm["posY"].as<double>() * mm;
		double z = vm["posZ"].as<double>() * mm;
		pos = G4ThreeVector(x, y, z);
	}
}

ParticleSourceMessenger* ParticleSourceMessenger::getInstance() {
	static ParticleSourceMessenger* instance = new ParticleSourceMessenger;
	return instance;
}

int ParticleSourceMessenger::getVerbose() const {
	return verbose;
}

void ParticleSourceMessenger::setVerbose(int _verbose) {
	verbose = _verbose;
}

int ParticleSourceMessenger::getNParticles() const {
	return nParticles;
}

void ParticleSourceMessenger::setNParticles(int _nParticles) {
	nParticles = _nParticles;
}

double ParticleSourceMessenger::getPolar() const {
	return polar;
}

void ParticleSourceMessenger::setPolar(double _polar) {
	polar = _polar;
}

double ParticleSourceMessenger::getEMax() const {
	return eMax;
}

void ParticleSourceMessenger::setEMax(double _eMax) {
	eMax = _eMax;
}

double ParticleSourceMessenger::getEMin() const {
	return eMin;
}

void ParticleSourceMessenger::setEMin(double _eMin) {
	eMin = _eMin;
}

double ParticleSourceMessenger::getPhiMax() const {
	return phiMax;
}

void ParticleSourceMessenger::setPhiMax(double _phiMax) {
	phiMax = _phiMax;
}

double ParticleSourceMessenger::getPhiMin() const {
	return phiMin;
}

void ParticleSourceMessenger::setPhiMin(double _phiMin) {
	phiMin = _phiMin;
}

double ParticleSourceMessenger::getThetaMax() const {
	return thetaMax;
}

void ParticleSourceMessenger::setThetaMax(double _thetaMax) {
	thetaMax = _thetaMax;
}

double ParticleSourceMessenger::getThetaMin() const {
	return thetaMin;
}

void ParticleSourceMessenger::setThetaMin(double _thetaMin) {
	thetaMin = _thetaMin;
}

G4ThreeVector ParticleSourceMessenger::getPos() const {
	return pos;
}

void ParticleSourceMessenger::setPos(G4ThreeVector _pos) {
	pos = _pos;
}

G4ThreeVector ParticleSourceMessenger::getSurfaceNormal() const {
	return surfaceNormal;
}

void ParticleSourceMessenger::setSurfaceNormal(G4ThreeVector _surfaceNormal) {
	surfaceNormal = _surfaceNormal;
}

double ParticleSourceMessenger::getA() const {
	return a;
}

void ParticleSourceMessenger::setA(double _a) {
	a = _a;
}

double ParticleSourceMessenger::getB() const {
	return b;
}

void ParticleSourceMessenger::setB(double _b) {
	b = _b;
}

double ParticleSourceMessenger::getTMin() const {
	return tMin;
}

void ParticleSourceMessenger::setTMin(double _tMin) {
	tMin = _tMin;
}

double ParticleSourceMessenger::getTMax() const {
	return tMax;
}

void ParticleSourceMessenger::setTMax(double _tMax) {
	tMax = _tMax;
}

G4String ParticleSourceMessenger::getTInput() const {
	return tInput;
}

G4String ParticleSourceMessenger::getEInput() const {
	return eInput;
}
