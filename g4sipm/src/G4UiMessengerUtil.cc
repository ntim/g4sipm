/*
 * G4UiMessengerUtil.cc
 *
 * @created Jan 17, 2013
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#include "G4UiMessengerUtil.hh"

G4UiMessengerUtil::G4UiMessengerUtil() {
	//
}

G4UIcmdWithABool* G4UiMessengerUtil::createCmd(G4UImessenger* messenger, G4String path, G4String name,
		bool defaultValue) {
	G4UIcmdWithABool* cmd = new G4UIcmdWithABool((path + name).data(), messenger);
	cmd->SetDefaultValue(defaultValue);
	return cmd;
}

G4UIcmdWithAnInteger* G4UiMessengerUtil::createCmd(G4UImessenger* messenger, G4String path, G4String name,
		int defaultValue) {
	G4UIcmdWithAnInteger* cmd = new G4UIcmdWithAnInteger((path + name).data(), messenger);
	cmd->SetDefaultValue(defaultValue);
	return cmd;
}

G4UIcmdWithAnInteger* G4UiMessengerUtil::createCmd(G4UImessenger* messenger, G4String path, G4String name,
		unsigned int defaultValue) {
	G4UIcmdWithAnInteger* cmd = new G4UIcmdWithAnInteger((path + name).data(), messenger);
	cmd->SetDefaultValue(defaultValue);
	return cmd;
}

G4UIcmdWithADouble* G4UiMessengerUtil::createCmd(G4UImessenger* messenger, G4String path, G4String name,
		double defaultValue) {
	G4UIcmdWithADouble* cmd = new G4UIcmdWithADouble((path + name).data(), messenger);
	cmd->SetDefaultValue(defaultValue);
	return cmd;
}

G4UIcmdWithAString* G4UiMessengerUtil::createCmd(G4UImessenger* messenger, G4String path, G4String name,
		G4String defaultValue) {
	G4UIcmdWithAString* cmd = new G4UIcmdWithAString((path + name).data(), messenger);
	cmd->SetDefaultValue(defaultValue);
	return cmd;
}

G4UIcmdWithADoubleAndUnit* G4UiMessengerUtil::createCmd(G4UImessenger* messenger, G4String path, G4String name,
		double defaultValue, G4String defaultUnit) {
	G4UIcmdWithADoubleAndUnit* cmd = new G4UIcmdWithADoubleAndUnit((path + name).data(), messenger);
	cmd->SetDefaultValue(defaultValue);
	cmd->SetDefaultUnit(defaultUnit);
	return cmd;
}

G4UIcmdWith3VectorAndUnit* G4UiMessengerUtil::createCmd(G4UImessenger* messenger, G4String path, G4String name,
		G4ThreeVector defaultValue, G4String defaultUnit) {
	G4UIcmdWith3VectorAndUnit* cmd = new G4UIcmdWith3VectorAndUnit((path + name).data(), messenger);
	cmd->SetDefaultValue(defaultValue);
	cmd->SetDefaultUnit(defaultUnit);
	return cmd;
}

void G4UiMessengerUtil::setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWithABool* out, const G4String& newValue,
		bool* value) {
	if (in == out) {
		*value = out->GetNewBoolValue(newValue);
	}
}

void G4UiMessengerUtil::setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWithAnInteger* out, const G4String& newValue,
		int* value) {
	if (in == out) {
		*value = out->GetNewIntValue(newValue);
	}
}

void G4UiMessengerUtil::setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWithAnInteger* out, const G4String& newValue,
		unsigned int* value) {
	if (in == out) {
		*value = out->GetNewIntValue(newValue);
	}
}

void G4UiMessengerUtil::setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWithADouble* out, const G4String& newValue,
		double* value) {
	if (in == out) {
		*value = out->GetNewDoubleValue(newValue);
	}
}

void G4UiMessengerUtil::setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWithAString* out, const G4String& newValue,
		G4String* value) {
	if (in == out) {
		*value = newValue;
	}
}

void G4UiMessengerUtil::setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWithADoubleAndUnit* out,
		const G4String& newValue, double* value) {
	if (in == out) {
		*value = out->GetNewDoubleValue(newValue);
	}
}

void G4UiMessengerUtil::setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWith3VectorAndUnit* out,
		const G4String& newValue, G4ThreeVector* value) {
	if (in == out) {
		*value = out->GetNew3VectorValue(newValue);
	}
}

void G4UiMessengerUtil::executeMacro(std::string filename, bool silent) {
	// Load settings from the macro file.
	std::fstream f;
	f.open(filename.c_str(), std::ios::in);
	if (f.is_open()) {
		G4UImanager::GetUIpointer()->ExecuteMacroFile(filename.c_str());
	} else if (!silent) {
		std::cerr << "G4UiMessengerUtil::executeMacro(" << filename << "): file not found." << std::endl;
	}
	f.close();
}
