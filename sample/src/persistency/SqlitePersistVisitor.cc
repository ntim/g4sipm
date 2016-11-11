/*
 * SqlitePersistVisitor.cc
 *
 * @date Feb 11, 2014
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "persistency/SqlitePersistVisitor.hh"

#include <iomanip>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include "persistency/PersistencyHandlerMessenger.hh"

int SqlitePersistVisitor::callback(void*, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

void SqlitePersistVisitor::exec(std::string query) {
	char *zErrMsg = 0;
	int rc = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\nQuery: %s\n", zErrMsg, query.c_str());
		sqlite3_free(zErrMsg);
	}
}

void SqlitePersistVisitor::open(std::string filename) {
	if (!boost::algorithm::ends_with(filename, ".sqlite")) {
		filename.append(".sqlite");
	}
	remove(filename.c_str());
	if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
		std::cerr << "SqlitePersistVisitor::open(filename = \"" << filename << "\"): could not open the ." << std::endl;
		throw 1;
	}
	// Disable synchronous mode to improve performance.
	exec("PRAGMA synchronous = 0;");
}

void SqlitePersistVisitor::operator ()(G4SipmHitsCollection* hc) {
	if (!PersistencyHandlerMessenger::getInstance()->isPersistHits()) {
		return;
	}
	exec(boost::str(boost::format("CREATE TABLE IF NOT EXISTS `%s` "
			"(sipmId INTEGER, pdgId INTEGER, trackId INTEGER, eKin REAL, time REAL, weight REAL, "
			"momentum TEXT, position TEXT, worldPosition TEXT, "
			"startMomentum TEXT, startPosition TEXT);") % hc->GetName()));
	exec("BEGIN TRANSACTION;");
	for (size_t i = 0; i < hc->GetSize(); i++) {
		(*this)((G4SipmHit*) hc->GetHit(i), hc);
	}
	exec("COMMIT;");
}

void SqlitePersistVisitor::operator ()(G4SipmDigiCollection* dc) {
	if (!PersistencyHandlerMessenger::getInstance()->isPersistDigis()) {
		return;
	}
	exec(boost::str(boost::format("CREATE TABLE IF NOT EXISTS `%s` "
			"(sipmId INTEGER, cellId INTEGER, time REAL, type INTEGER, weight REAL);") % dc->GetName()));
	const double tMin = PersistencyHandlerMessenger::getInstance()->getTMin();
	const double tMax = PersistencyHandlerMessenger::getInstance()->getTMax();
	exec("BEGIN TRANSACTION;");
	for (size_t i = 0; i < dc->GetSize(); i++) {
		G4SipmDigi* digi = (G4SipmDigi*) (dc->GetDigi(i));
		if (digi->getTime() >= tMin && digi->getTime() < tMax) {
			(*this)(digi, dc);
		}
	}
	exec("COMMIT;");
}

void SqlitePersistVisitor::operator ()(G4SipmVoltageTraceDigiCollection* dc) {
	exec(boost::str(boost::format("CREATE TABLE IF NOT EXISTS `%s` "
			"(sipmId INTEGER, timeBinWidth REAL, time REAL, voltage REAL);") % dc->GetName()));
	exec("BEGIN TRANSACTION;");
	for (size_t i = 0; i < dc->GetSize(); i++) {
		(*this)((G4SipmVoltageTraceDigi*) dc->GetDigi(i), dc);
	}
	exec("COMMIT;");
}

void SqlitePersistVisitor::operator ()(G4SipmHit* hit, G4SipmHitsCollection* hc) {
	std::stringstream q;
	q << std::setprecision(9) << "INSERT INTO `" << hc->GetName() << "` VALUES (" << hit->getSipmId() << ","
			<< hit->getPdgId() << "," << hit->getTrackId() << "," << hit->getEKin() << "," << hit->getTime() << ","
			<< hit->getWeight() << ",\"" << hit->getMomentum() << "\",\"" << hit->getPosition() << "\",\""
			<< hit->getWorldPosition() << "\",\"" << hit->getStartMomentum() << "\",\"" << hit->getStartPosition()
			<< "\");";
	exec(q.str());
}

void SqlitePersistVisitor::operator ()(G4SipmDigi* digi, G4SipmDigiCollection* dc) {
	std::stringstream q;
	q << std::setprecision(9) << "INSERT INTO `" << dc->GetName() << "` VALUES (" << digi->getSipmId() << ","
			<< digi->getCellId() << "," << digi->getTime() << "," << digi->getType() << "," << digi->getWeight()
			<< ");";
	exec(q.str());
}

void SqlitePersistVisitor::operator ()(G4SipmVoltageTraceDigi* digi, G4SipmVoltageTraceDigiCollection* dc) {
	const double tMin = std::max(digi->getTMin(), PersistencyHandlerMessenger::getInstance()->getTMin());
	const double tMax = std::min(digi->getTMax(), PersistencyHandlerMessenger::getInstance()->getTMax());
	for (double t = tMin; t < tMax; t += digi->getTimeBinWidth()) {
		std::stringstream q;
		q << "INSERT INTO `" << dc->GetName() << "` VALUES (" << digi->getSipmId() << "," << digi->getTimeBinWidth()
				<< "," << t << "," << digi->atMeasured(digi->index(t)) << ");";
		exec(q.str());
	}
}

void SqlitePersistVisitor::close() {
	sqlite3_close(db);
}

void SqlitePersistVisitor::operator ()(G4SipmUiMessenger* m) {
	exec(
			"CREATE TABLE IF NOT EXISTS g4sipmUiMessenger "
					"(verbose INTEGER, seed INTEGER, filterGeometry INTEGER, filterPde INTEGER, filterTiming INTEGER,"
					"noiseThermal INTEGER, noisePreThermal INTEGER, noisePostThermal INTEGER, noiseAfterpulse INTEGER,"
					"noiseCrosstalk INTEGER, noiseCrosstalkNeighbours INTEGER, noiseIfNoSignal INTEGER, digitizeHits INTEGER, digitizeTrace INTEGER,"
					"simulateShuntresistor INTEGER,	shuntresistorRecoveryTime REAL);");
	exec(
			boost::str(
					boost::format(
							"INSERT INTO g4sipmUiMessenger VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%g);")
							% m->getVerbose() % m->getSeed() % m->isFilterGeometry() % m->isFilterPde()
							% m->isFilterTiming() % m->isNoiseThermal() % m->getNoisePreThermal()
							% m->getNoisePostThermal() % m->isNoiseAfterpulse() % m->isNoiseCrosstalk()
							% m->getNoiseCrosstalkNeighbours() % m->isNoiseIfNoSignal() % m->isDigitizeHits()
							% m->isDigitizeTrace() % m->isSimulateShuntresistor() % m->getShuntresistorRecoveryTime()));
}

void SqlitePersistVisitor::operator ()(ParticleSourceMessenger* m) {
	exec("CREATE TABLE IF NOT EXISTS particleSourceMessenger "
			"(verbose INTEGER, nParticles INTEGER, polar REAL, tMin REAL, tMax REAL, tInput TEXT, "
			"eMin REAL, eMax REAL, eInput TEXT, phiMin REAL, phiMax REAL, thetaMin REAL, thetaMax REAL, "
			"surfaceNormal TEXT, a REAL, b REAL, pos TEXT);");
	exec(
			boost::str(
					boost::format(
							"INSERT INTO particleSourceMessenger VALUES (%d, %d, %g, %g, %g, \"%s\", %g, %g,\"%s\", %g, %g, %g, %g, \"%s\", %g, %g, \"%s\");")
							% m->getVerbose() % m->getNParticles() % m->getPolar() % m->getTMin() % m->getTMax()
							% m->getTInput() % m->getEMin() % m->getEMax() % m->getEInput() % m->getPhiMin()
							% m->getPhiMax() % m->getThetaMin() % m->getThetaMax() % m->getSurfaceNormal() % m->getA()
							% m->getB() % m->getPos()));
}

void SqlitePersistVisitor::operator ()(G4SipmModel* m) {
	exec("CREATE TABLE IF NOT EXISTS sipmModel "
			"(name TEXT, numberOfCells INTEGER, cellPitch REAL, pitch REAL, fillFactor REAL, "
			"temperature REAL, overVoltage REAL, breakdownVoltage REAL, "
			"thermalNoiseRate REAL, crossTalkProbability REAL, "
			"apProbLong REAL, apProbShort REAL, apTauLong REAL, apTauShort REAL, "
			"recoveryTime REAL, deadTime REAL, pdeAt400nm REAL);");
	exec(
			boost::str(
					boost::format(
							"INSERT INTO sipmModel VALUES(\"%s\", %d, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g);")
							% m->getName() % m->getNumberOfCells() % m->getCellPitch() % m->getPitch()
							% m->getFillFactor() % m->getTemperature() % m->getOverVoltage() % m->getBreakdownVoltage()
							% m->getThermalNoiseRate() % m->getCrossTalkProbability() % m->getApProbLong()
							% m->getApProbShort() % m->getApTauLong() % m->getApTauShort() % m->getRecoveryTime()
							% m->getDeadTime() % m->getPhotonDetectionEfficiency(400 * CLHEP::nm)));
}

void SqlitePersistVisitor::operator ()(G4SipmVoltageTraceModel* m) {
	exec("CREATE TABLE IF NOT EXISTS sipmVoltageTraceModel "
			"(amplitude REAL, tauFall REAL, tauRise REAL, timeBinWidht REAL, v0 REAL, whiteNoiseSigma REAL);");
	exec(
			boost::str(
					boost::format("INSERT INTO sipmVoltageTraceModel VALUES(%g, %g, %g, %g, %g, %g);")
							% m->getAmplitude() % m->getTauFall() % m->getTauRise() % m->getTimeBinWidth() % m->getV0()
							% m->getWhiteNoiseSigma()));
}
