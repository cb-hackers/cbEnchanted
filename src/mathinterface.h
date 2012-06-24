#ifndef MATHINTERFACE_H
#define MATHINTERFACE_H

#include "precomp.h"

class CBEnchanted;

class MathInterface {
	public:
		MathInterface();
#ifndef CBE_LIB
		void commandRandomize(void);

		void functionInt(void);
		void functionFloat(void);
		void functionRoundUp(void);
		void functionRoundDown(void);
		void functionAbs(void);
		void functionSqrt(void);
		void functionSin(void);
		void functionCos(void);
		void functionTan(void);
		void functionASin(void);
		void functionACos(void);
		void functionATan(void);
		void functionGetAngle(void);
		void functionLog(void);
		void functionLog10(void);
		void functionRnd(void);
		void functionRand(void);
		void functionMin(void);
		void functionMax(void);
		void functionCurveValue(void);
		void functionCurveAngle(void);
		void functionWrapAngle(void);
		void functionDistance(void);
		void functionBoxOverlap(void);
#endif
		static float wrapAngle(float v);
		static int32_t wrapAngle(int32_t v);
		static double square(float v);
	private:
		CBEnchanted *cb;
};

inline
float MathInterface::wrapAngle(float v) {
	while(v > 360.0f) {
		v -= 360.0f;
	}
	while (v < 0.0f) {
		v += 360.0f;
	}
	return v;
}

inline
int32_t MathInterface::wrapAngle(int32_t v) {
	while(v > 360.0) {
		v -= 360.0;
	}
	while (v < 0.0) {
		v += 360.0;
	}
	return v;
}

inline
double MathInterface::square(float v) {
	return (double)v * (double)v;
}

#endif
