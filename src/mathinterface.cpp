#include "precomp.h"
#include "cbemu.h"
#include "mathinterface.h"

MathInterface::MathInterface() {
	cb = static_cast <CBEmu *> (this);
}
		
void MathInterface::functionWrapAngle(void) {
	float angle = cb->popValue<float>();
	while (angle > 360.0) {
		angle -= 360.0;
	}
	while (angle < 0.0) {
		angle += 360.0;
	}

	cb->pushValue(angle);
}

void MathInterface::functionCos(void) {
	float angle = cb->popValue<float>();
	
	cb->pushValue(float(cos(angle / 180.0 * 3.14159265358979323)));
}

void MathInterface::functionSin(void) {
	float angle = cb->popValue<float>();
	
	cb->pushValue(float(sin(angle / 180.0 * 3.14159265358979323)));
}

