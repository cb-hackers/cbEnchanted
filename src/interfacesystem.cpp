#include "interfacesystem.h"

InterfaceSystem::InterfaceSystem() {
}

bool InterfaceSystem::init() {
	if (!initializeGfx()) {
		return false;
	}
	if (!initializeInputs()) {
		INFO("InitializeInputs failed");
		return false;
	}
	if (!initializeSounds()){
		INFO("InitializeSounds failed");
		return false;
	}
	if (!initializeFonts()) {
		INFO("initializeFonts failed");
		return false;
	}
	return true;
}
