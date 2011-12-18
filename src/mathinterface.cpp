#include "precomp.h"
#include "cbemu.h"
#include "mathinterface.h"

MathInterface::MathInterface() {
	cb = static_cast <CBEmu *> (this);
}

void MathInterface::commandRandomize(void) {
	
}

void MathInterface::functionInt(void) {
	
}

void MathInterface::functionFloat(void) {
	
}

void MathInterface::functionRoundUp(void) {
	
}

void MathInterface::functionRoundDown(void) {
	
}

void MathInterface::functionAbs(void) {
	
}

void MathInterface::functionSqrt(void) {
	
}

void MathInterface::functionSin(void) {
	float angle = cb->popValue<float>();
	
	cb->pushValue(float(sin(angle / 180.0 * 3.14159265358979323)));
}

void MathInterface::functionCos(void) {
	float angle = cb->popValue<float>();
	
	cb->pushValue(float(cos(angle / 180.0 * 3.14159265358979323)));
}

void MathInterface::functionTan(void) {
	
}

void MathInterface::functionASin(void) {
	
}

void MathInterface::functionACos(void) {
	
}

void MathInterface::functionATan(void) {
	
}

void MathInterface::functionGetAngle(void) {
	
}

void MathInterface::functionLog(void) {
	
}

void MathInterface::functionLog10(void) {
	
}

void MathInterface::functionRnd(void) {
	
}

void MathInterface::functionRand(void) {
	
}

void MathInterface::functionMin(void) {
	
}

void MathInterface::functionMax(void) {
	
}

void MathInterface::functionCurveValue(void) {
	
}

void MathInterface::functionCurveAngle(void) {
	
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

void MathInterface::functionDistance(void) {
	
}

void MathInterface::functionBoxOverlap(void) {
	
}
