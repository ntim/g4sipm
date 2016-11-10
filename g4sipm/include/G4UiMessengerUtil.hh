/*
 * G4UiMessengerUtil.hh
 *
 * @date 22.08.2012
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef G4UIMESSENGERUTIL_HH_
#define G4UIMESSENGERUTIL_HH_

#include <fstream>
#include <G4UImessenger.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4UImanager.hh>

/**
 * Utility class providing easier handling of UI commands.
 */
class G4UiMessengerUtil {
private:
	G4UiMessengerUtil();

public:
	/**
	 * @param messenger - the messenger instance.
	 * @param path - the command path.
	 * @param name - the command name.
	 * @param defaultValue - the default value to set.
	 * @return G4UIcmdWithABool - the command.
	 */
	static G4UIcmdWithABool* createCmd(G4UImessenger* messenger, G4String path, G4String name, bool defaultValue);

	/**
	 * @param messenger - the messenger instance.
	 * @param path - the command path.
	 * @param name - the command name.
	 * @param defaultValue - the default value to set.
	 * @return G4UIcmdWithAnInteger - the command.
	 */
	static G4UIcmdWithAnInteger* createCmd(G4UImessenger* messenger, G4String path, G4String name, int defaultValue);

	/**
	 * @param messenger - the messenger instance.
	 * @param path - the command path.
	 * @param name - the command name.
	 * @param defaultValue - the default value to set.
	 * @return G4UIcmdWithAnInteger - the command.
	 */
	static G4UIcmdWithAnInteger* createCmd(G4UImessenger* messenger, G4String path, G4String name,
			unsigned int defaultValue);

	/**
	 * @param messenger - the messenger instance.
	 * @param path - the command path.
	 * @param name - the command name.
	 * @param defaultValue - the default value to set.
	 * @return G4UIcmdWithADouble - the command.
	 */
	static G4UIcmdWithADouble* createCmd(G4UImessenger* messenger, G4String path, G4String name, double defaultValue);

	/**
	 * @param messenger - the messenger instance.
	 * @param path - the command path.
	 * @param name - the command name.
	 * @param defaultValue - the default value to set.
	 * @return G4UIcmdWithAString - the command.
	 */
	static G4UIcmdWithAString* createCmd(G4UImessenger* messenger, G4String path, G4String name, G4String defaultValue);

	/**
	 * @param messenger - the messenger instance.
	 * @param path - the command path.
	 * @param name - the command name.
	 * @param defaultValue - the default value to set.
	 * @param defaultUnit - the default unit.
	 * @return G4UIcmdWithADoubleAndUnit - the command.
	 */
	static G4UIcmdWithADoubleAndUnit* createCmd(G4UImessenger* messenger, G4String path, G4String name,
			double defaultValue, G4String defaultUnit);

	/**
	 * @param messenger - the messenger instance.
	 * @param path - the command path.
	 * @param name - the command name.
	 * @param defaultValue - the default value to set.
	 * @param defaultUnit - the default unit.
	 * @return G4UIcmdWith3VectorAndUnit - the command.
	 */
	static G4UIcmdWith3VectorAndUnit* createCmd(G4UImessenger* messenger, G4String path, G4String name,
			G4ThreeVector defaultValue, G4String defaultUnit);

	/**
	 * @param in - the command of the new value.
	 * @param out - the command the value corresponds to.
	 * @param newValue - the new value string.
	 * @param value - the value to set.
	 */
	static void setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWithABool* out, const G4String& newValue, bool* value);

	/**
	 * @param in - the command of the new value.
	 * @param out - the command the value corresponds to.
	 * @param newValue - the new value string.
	 * @param value - the value to set.
	 */
	static void setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWithAnInteger* out, const G4String& newValue,
			int* value);

	/**
	 * @param in - the command of the new value.
	 * @param out - the command the value corresponds to.
	 * @param newValue - the new value string.
	 * @param value - the value to set.
	 */
	static void setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWithAnInteger* out, const G4String& newValue,
			unsigned int* value);

	/**
	 * @param in - the command of the new value.
	 * @param out - the command the value corresponds to.
	 * @param newValue - the new value string.
	 * @param value - the value to set.
	 */
	static void setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWithADouble* out, const G4String& newValue,
			double* value);

	/**
	 * @param in - the command of the new value.
	 * @param out - the command the value corresponds to.
	 * @param newValue - the new value string.
	 * @param value - the value to set.
	 */
	static void setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWithAString* out, const G4String& newValue,
			G4String* value);

	/**
	 * @param in - the command of the new value.
	 * @param out - the command the value corresponds to.
	 * @param newValue - the new value string.
	 * @param value - the value to set.
	 */
	static void setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWithADoubleAndUnit* out, const G4String& newValue,
			double* value);

	/**
	 * @param in - the command of the new value.
	 * @param out - the command the value corresponds to.
	 * @param newValue - the new value string.
	 * @param value - the value to set.
	 */
	static void setNewValueIfCmdMatches(G4UIcommand* in, G4UIcmdWith3VectorAndUnit* out, const G4String& newValue,
			G4ThreeVector* value);

	/**
	 * Executes a macro file.
	 *
	 * @param filename - the macro file name.
	 * @param silent - if true, no error message will occur if the file does not exist.
	 */
	static void executeMacro(std::string filename, bool silent = false);

};

#endif /* G4UIMESSENGERUTIL_HH_ */
