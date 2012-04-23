#include "interfacesystem.h"
InterfaceSystem *interfaceSystemInstance  = 0;

InterfaceSystem::InterfaceSystem() {
	assert(interfaceSystemInstance == 0 && "Multiple InterfaceSystems");
	interfaceSystemInstance = this;
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

InterfaceSystem *InterfaceSystem::instance() {
	return interfaceSystemInstance;
}
