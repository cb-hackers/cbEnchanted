#include "interfacecaller.h"
#include "interfacesystem.h"
#include "cbvariableholder.h"

InterfaceCaller::InterfaceCaller(CBVariableHolder *s):
	system( new InterfaceSystem),
	stack(s)
{
}

bool InterfaceCaller::init() {
	return system->init();
}

void InterfaceCaller::commandDrawScreen() {
	uint32_t state = stack->popValue().toInt();
	uint32_t depth = stack->popValue().toInt();
	uint32_t height = stack->popValue().toInt();
	uint32_t width = stack->popValue().toInt();
	system->drawScreen(width, height, depth, state);
}
