#ifndef MATHINTERFACE_H
#define MATHINTERFACE_H

class CBEnchanted;

class MathInterface {
	public:
		MathInterface();

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
	private:
		InterfaceSystem *sys;
};

#endif
