#ifndef MATHINTERFACE_H
#define MATHINTERFACE_H

class CBEmu;

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
		void functionAsin(void);
		void functionAcos(void);
		void functionAtan(void);
		
		void functionWrapAngle(void);
	private:
		CBEmu *cb;
};

#endif
