#ifndef MATHINTERFACE_H
#define MATHINTERFACE_H

#include "precomp.h"

class CBEnchanted;

/** General mathematic interface that uses degrees, not radians. */
class MathInterface {
	public:
		MathInterface();

		inline void randomize(int32_t seed);

		inline int32_t abs(int32_t value);
		inline float abs(float value);

		inline float asin(float value);
		inline float atan(float value);
		inline float acos(float value);
		inline float sin(float angle);
		inline float tan(float angle);
		inline float cos(float angle);

		int32_t wrapAngle(int32_t angle);
		float wrapAngle(float angle);

		float curveValue(float newV, float oldV, float smoothness);
		float curveAngle(float newA, float oldA, float smoothness);

		inline float sqrt(float value);
		inline float log(float value);
		inline float log10(float value);

		inline float rand(float low, float high);
		inline int32_t rand(int32_t low, int32_t high);

		int32_t roundUp(float value);
		int32_t roundDown(float value);

		float getAngle(float x1, float y1, float x2, float y2);
		float distance(float x1, float y1, float x2, float y2);
		bool boxOverlap(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

	private:
		//Helper
		inline double square(float num) {
			return (double)num * (double)num;
		}
};

#endif
