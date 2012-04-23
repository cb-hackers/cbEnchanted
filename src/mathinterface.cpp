#include "precomp.h"
#include "mathinterface.h"
#include <math.h>
#include "util.h"

MathInterface::MathInterface() {
	// Nothing needs constructing
}

void MathInterface::randomize(int32_t seed) {
	srand(seed);
}


int32_t MathInterface::roundUp(float value) {
	return int32_t(ceil(value));
}

int32_t MathInterface::roundDown(float value) {
	return int32_t(floor(value));
}

int32_t MathInterface::abs(int32_t value) {
	return abs(value);
}

float MathInterface::abs(float value) {
	return fabs(value);
}

float MathInterface::sqrt(float value) {
	return sqrt(value);
}

float MathInterface::sin(float angle) {
	return sin(angle / 180.0f * M_PI);
}

float MathInterface::cos(float angle) {
	return cos(angle / 180.0f * M_PI);
}

float MathInterface::tan(float angle) {
	return tan(angle / 180.0f * M_PI);
}

float MathInterface::asin(float value) {
	return asinf(value) / M_PI * 180.0f;
}

float MathInterface::acos(float value) {
	return acosf(value) / M_PI * 180.0f;
}

float MathInterface::atan(float value) {
	return atanf(value) / M_PI * 180.0f;
}

float MathInterface::getAngle(float x1, float y1, float x2, float y2) {
	return (M_PI - atan2f(y1 - y2, x1 - x2)) / M_PI * 180.0f;
}

float MathInterface::log(float value) {
	return logf(value);
}

float MathInterface::log10(float value) {
	return log10f(v);
}

float MathInterface::rand(float low, float high) {
	if (high < low) {
		return randf() * low;
	}
	return low + (randf() * (high - low));
}

int32_t MathInterface::rand(int32_t low, int32_t high) {
	if (high < low) {cb->pushValue(rand(low)); return;}
	cb->pushValue(low + rand(high - low));
}

float MathInterface::curveValue(float newV, float oldV, float smoothness) {
	return oldV + (newV - oldV) / smoothness;
}

float MathInterface::curveAngle(float newA, float oldA, float smoothness) {
	oldA = wrapAngle(oldA);
	newA = wrapAngle(newA);

	if (abs(newA - oldA) < (360.0 - newA) + oldA) {
		return oldA + (newA - oldA) / smoothness;
	}
	else {
		return (oldA - ((360.0f - newA) + oldA) / smoothness) + 360.0f;
	}
}

float MathInterface::wrapAngle(float angle) {
	while (angle > 360.0f) {
		angle -= 360.0f;
	}
	while (angle < 0.0f) {
		angle += 360.0f;
	}
	return angle;
}

int32_t MathInterface::wrapAngle(int32_t angle) {
	while (angle > 360) {
		angle -= 360;
	}
	while (angle < 0) {
		angle += 360;
	}
	return angle;
}

float MathInterface::distance(float x1, float y1, float x2, float y2) {
	return float(sqrt(square(x2 - x1) + square(y2 - y1)));
}

bool MathInterface::boxOverlap(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	return (abs(y1 - y2) < (h1 + h2)) && ((abs(x1 - x2) < (w1 + w2)));
}
