#include "precomp.h"
#include "cbenchanted.h"
#include "mathinterface.h"
#include <math.h>
#include "mathoperations.h"
#include "util.h"

MathInterface::MathInterface() {
	cb = static_cast <CBEnchanted *> (this);
}

void MathInterface::commandRandomize(void) {
	srand(cb->popValue().getInt());
}

void MathInterface::functionInt(void) {
	int v = cb->popValue().toInt();
	cb->pushValue(v);
}

void MathInterface::functionFloat(void) {
	float v = cb->popValue().toFloat();
	cb->pushValue(v);
}

void MathInterface::functionRoundUp(void) {
	float v = cb->popValue().toFloat();
	cb->pushValue((int32_t)ceil(v));
}

void MathInterface::functionRoundDown(void) {
	float v = cb->popValue().toFloat();
	cb->pushValue((int32_t)floor(v));
}

void MathInterface::functionAbs(void)
{
	Any v = cb->popValue();

	if (v.type() == Any::Float)
	{
		cb->pushValue(abs(v.getFloat()));
		return;
	}
	if (v.type() == Any::Int) //TODO: Check if necessary
	{
		cb->pushValue(abs(v.getInt()));
	}
	cb->pushValue(v);
}

void MathInterface::functionSqrt(void) {
	float v = cb->popValue().toFloat();
	cb->pushValue(sqrt(v));
}

void MathInterface::functionSin(void) {
	float angle = cb->popValue().toFloat();

	cb->pushValue(float(sin(angle / 180.0 * 3.14159265358979323)));
}

void MathInterface::functionCos(void) {
	float angle = cb->popValue().toFloat();

	cb->pushValue(float(cos(angle / 180.0 * 3.14159265358979323)));
}

void MathInterface::functionTan(void) {
	float angle = cb->popValue().toFloat();
	cb->pushValue(float(tan(angle / 180.0 * 3.14159265358979323)));
}

void MathInterface::functionASin(void) {
	float v = cb->popValue().toFloat();
	cb->pushValue((float)(asinf(v) / 3.14159265358979323 * 180.0));
}

void MathInterface::functionACos(void) {
	float v = cb->popValue().toFloat();
	cb->pushValue((float)(acosf(v) / 3.14159265358979323 * 180.0));
}

void MathInterface::functionATan(void) {
	float v = cb->popValue().toFloat();
	cb->pushValue((float)(atanf(v) / 3.14159265358979323 * 180.0));
}

void MathInterface::functionGetAngle(void) {
	float y2 = cb->popValue().toFloat();
	float x2 = cb->popValue().toFloat();
	float y1 = cb->popValue().toFloat();
	float x1 = cb->popValue().toFloat();
	cb->pushValue((float)((3.14159265358979323 - atan2f(y1 - y2, x1 - x2)) / 3.14159265358979323 * 180.0));
}

void MathInterface::functionLog(void) {
	float v = cb->popValue().toFloat();
	cb->pushValue(logf(v));
}

void MathInterface::functionLog10(void) {
	float v = cb->popValue().toFloat();
	cb->pushValue(log10f(v));
}

void MathInterface::functionRnd(void) {
	float high = cb->popValue().toFloat();
	float low = cb->popValue().toFloat();
	if (high < low) {cb->pushValue(randf()*low);return;}
	cb->pushValue(low + (randf() * (high - low)));
}

void MathInterface::functionRand(void) {
	int32_t high = cb->popValue().toInt();
	int32_t low = cb->popValue().toInt();
	if (high < low) {cb->pushValue(rand(low)); return;}
	cb->pushValue(low + rand(high - low));
}

void MathInterface::functionMin(void) {
	Any b = cb->popValue();
	Any a = cb->popValue();
	cb->pushValue( (a < b) ? a : b);
}

void MathInterface::functionMax(void) {
	Any b = cb->popValue();
	Any a = cb->popValue();
	cb->pushValue( (a > b) ? a : b);
}

void MathInterface::functionCurveValue(void) {
	float smoothness = cb->popValue().toFloat();
	float oldV = cb->popValue().toFloat();
	float newV = cb->popValue().toFloat();
	cb->pushValue(oldV + (newV - oldV) / smoothness);
}

inline float wrapAngle(float v) {
	while(v > 360.0f) {
		v -= 360.0f;
	}
	while (v < 0.0f)
	{
		v += 360.0f;
	}
	return v;
}

void MathInterface::functionCurveAngle(void) {
	float smoothness = cb->popValue().toFloat();
	float oldA = cb->popValue().toFloat();
	float newA = cb->popValue().toFloat();
	oldA = wrapAngle(oldA);
	newA = wrapAngle(newA);

	if (abs(newA - oldA) < (360.0 - newA) + oldA)
	{
		cb->pushValue((oldA + (newA - oldA) / smoothness));
	}
	else
	{
		cb->pushValue((oldA - ((360.0f - newA) + oldA) / smoothness) + 360.0f);
	}

}

void MathInterface::functionWrapAngle(void) {
	Any a = cb->popValue();
	if (a.type() == Any::Float)
	{
		float angle = a.getFloat();
		while (angle > 360.0) {
			angle -= 360.0;
		}
		while (angle < 0.0) {
			angle += 360.0;
		}
		cb->pushValue(angle);
		return;
	}
	if (a.type() == Any::Int)
	{
		int32_t angle = a.getInt();
		while (angle > 360) {
			angle -= 360;
		}
		while (angle < 0) {
			angle += 360;
		}
		cb->pushValue(angle);
		return;
	}
	cb->pushValue(a);
}


//Helper
inline double square(float num) {
	return (double)num * (double)num;
}

void MathInterface::functionDistance(void) {
	float y2 = cb->popValue().toFloat();
	float x2 = cb->popValue().toFloat();
	float y1 = cb->popValue().toFloat();
	float x1 = cb->popValue().toFloat();
	cb->pushValue((float)sqrt(square(x2-x1)+square(y2-y1)));
}

void MathInterface::functionBoxOverlap(void) {
	float height2h = cb->popValue().toFloat() * 0.5f;
	float width2h = cb->popValue().toFloat() * 0.5f;
	float y2 = cb->popValue().toFloat();
	float x2 = cb->popValue().toFloat();
	float height1h = cb->popValue().toFloat() * 0.5f;
	float width1h = cb->popValue().toFloat() * 0.5f;
	float y1 = cb->popValue().toFloat();
	float x1 = cb->popValue().toFloat();

	cb->pushValue((abs(y1 - y2) < (height1h + height2h)) && ((abs(x1 - x2) < (width1h + width2h))) ? 1 : 0);
}
