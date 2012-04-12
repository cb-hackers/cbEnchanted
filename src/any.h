#ifndef ANY_H
#define ANY_H

#include "debug.h"
#include <assert.h>
#include "isstring.h"

class VariableStack;

class Any {
	public:
		/** Enumerated type for CB's data types
		  */
		enum Type {
			Empty = 0,
			Float = 1,
			Int = 2,
			String = 3,
			TypePtr = 4
		};
	private:
		union {
			/** Float data */
			float dFloat;
			/** 32bit integer data */
			int32_t dInt;
			/** ISString data */
			ISString::SharedData *dString;
			/** Type pointer */
			void *dPtr;
		};

		/** Any::Type of data type saved to Any
		  */
		uint8_t typeId;
	public:
		/** Constructs empty Any. empty() == true
		  */
		Any();
		/** Constructs Any with integer data
		  * @param a Integer to be stored in Any
		  */
		Any(int32_t a);
		/** Constructs Any with float data
		  * @param a Float to be stored in Any
		  */
		Any(float a);
		/** Constructs Any with type pointer data
		  * @param ptr Type pointer to be stored in Any
		  */
		Any(void *ptr);
		/** Constructs Any with string data
		  * @param a String to be stored in Any
		  */
		Any(const string &a);
		/** Constructs Any with string data
		  * @param a String to be stored in Any
		  */
		Any(const ISString &a);
		/** Copy constructor
		  * @param a Other Any
		  */
		Any(const Any &a);
		/** Destructor of Any.
		  */
		~Any();

		/** Assignment operator
		  */
		Any & operator = (const Any &a);

		/** Returns string stored in Any. Causes assertion if typeId isn't String
		  * @return String stored in Any
		  */
		inline ISString getString() const { assert(typeId == String); return ISString(dString); }
		/** Returns int stored in Any. Causes assertion if typeId isn't Int
		  * @return Int stored in Any
		  */
		inline int32_t getInt() const { assert(typeId == Int); return dInt; }
		/** Returns float stored in Any. Causes assertion if typeId isn't String
		  * @return Float stored in Any
		  */
		inline float getFloat() const { assert(typeId == Float); return dFloat; }
		/** Returns type pointer stored in Any. Causes assertion if typeId isn't TypePtr
		  * @return Type pointer stored in Any
		  */
		inline void *getTypePtr() const { assert(typeId == TypePtr); return dPtr;}
		/** Returns true if Any is constructed with default constructor.
		  * @return Returns true if Any is constructed with default constructor.
		  */
		inline bool empty() const { return typeId == Empty; }

		/**
		  * @return Any::Type
		  */
		int32_t type() const{ return typeId; }

		const type_info &typeInfo() const;

		/** Converts data to string and returns it
		  * @return Converted string value
		  */
		ISString toString() const;

		/** Converts data to int and returns it
		  * @return Converted int value
		  */
		int32_t toInt() const;

		/** Converts data to float and returns it
		  * @return Converted float value
		  */
		float toFloat() const;

		/** Converts data to short and returns it
		  * @return Converted short value
		  */
		uint16_t toShort() const;

		/** Converts data to byte and returns it
		  * @return Converted byte value
		  */
		uint8_t toByte() const;

		/** Negation operator */
		int32_t operator ! ()const;
		/** Unary plus operator */
		Any operator + ()const;
		/** Unary minus operator */
		Any operator - ()const;
		/** Modulo operator */
		Any operator % (const Any &r) const;
		/** Multiplication operator */
		Any operator * (const Any &r) const;
		/** Addition operator */
		Any operator + (const Any &r) const;
		/** Subtraction operator */
		Any operator - (const Any &r) const;
		/** Division operator */
		Any operator / (const Any &r) const;
		/** Bitwise left shift operator */
		Any operator << (const Any &r) const;
		/** Logical bitwise right shift operator */
		static int32_t shr (const Any &l, const Any &r);
		/** Arithmetic bitwise right shift operator */
		static int32_t sar (const Any &l, const Any &r);
		/** Power operator */
		Any operator ^ (const Any &r) const;
		/** Not equal operator */
		int32_t operator != (const Any &r) const;
		/** Logical AND operator */
		int32_t operator && (const Any &r) const;
		/** Less than or equal operator */
		int32_t operator <= (const Any &r) const;
		/** Equal operator */
		int32_t operator == (const Any &r) const;
		/** Greater than or equal operator */
		int32_t operator >= (const Any &r) const;
		/** Logical or operator */
		int32_t operator || (const Any &r) const;
		/** Greater than operator */
		int32_t operator > (const Any &r) const;
		/** Less than operator */
		int32_t operator < (const Any &r) const;

		/** Optimized version of addition for handleMathCommand */
		static void addition(VariableStack *s);

		/** Optimized version of multiplication for handleMathCommand */
		static void multiplication(VariableStack *s);

		/** Optimized version of division for handleMathCommand */
		static void division(VariableStack *s);

		/** Optimized version of subtraction for handleMathCommand */
		static void subtraction(VariableStack *s);

		/** Optimized version of modulo for handleMathCommand */
		static void modulo(VariableStack *s);

		/** Optimized version of bitwise left shift for handleMathCommand */
		static void shl(VariableStack *s);

		/** Optimized version of bitwise arithmetic right shift for handleMathCommand */
		static void sar(VariableStack *s);

		/** Optimized version of bitwise logical right shift for handleMathCommand */
		static void shr(VariableStack *s);
};



#endif // ANY_H
