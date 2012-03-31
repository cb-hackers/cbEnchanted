#ifndef ANY_H
#define ANY_H

#include "debug.h"
#include <assert.h>
#include "isstring.h"

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
		/** Any::Type of data type saved to Any
		  */
		int32_t typeId;

		union {
			/** Float data
			  */
			float dFloat;
			/** 32bit integer data
			  */
			int32_t dInt;
			/** ISString data
			  */
			ISString::SharedData *dString;
			/** Type pointer
			  */
			void *dPtr;
		};

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
		Any &operator = (const Any &a);

		/** Returns string stored in Any. Causes assertion if typeId isn't String
		  * @return String stored in Any
		  */
		ISString getString() const { assert(typeId == String); return ISString(dString); }
		/** Returns int stored in Any. Causes assertion if typeId isn't Int
		  * @return Int stored in Any
		  */
		int32_t getInt() const { assert(typeId == Int); return dInt; }
		/** Returns float stored in Any. Causes assertion if typeId isn't String
		  * @return Float stored in Any
		  */
		float getFloat() const { assert(typeId == Float); return dFloat; }
		/** Returns type pointer stored in Any. Causes assertion if typeId isn't TypePtr
		  * @return Type pointer stored in Any
		  */
		void *getTypePtr() { assert(typeId == TypePtr); return dPtr;}
		/** Returns true if Any is constructed with default constructor.
		  * @return Returns true if Any is constructed with default constructor.
		  */
		bool empty() const { return typeId == Empty; }

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
};

#endif // ANY_H
