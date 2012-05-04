#ifndef ISSTRING_H
#define ISSTRING_H
#include "precomp.h"

class Any;

/**
  * Implicit-shared string
  */
class ISString {
	public:
		friend class Any;

		/** Constructs a null ISString */
		ISString() : data(0){ }

		/** Constructs an ISString from a null terminated string. */
		ISString(const char *str);

		/** Constructs an ISString from a std::string */
		ISString(const string &str);

		/** Constructs an ISString from a ALLEGRO_USTR. ISString takes ownership of ALLEGRO_USTR. */
		ISString(ALLEGRO_USTR *str);

		/** Copy constructor */
		ISString(const ISString &str);

		/** Destructor */
		~ISString();

		/** Assignment operator */
		ISString & operator=(const ISString &o);

		/** Equal to operator */
		bool operator == (const ISString &o) const;

		/** Not equal to operator */
		bool operator != (const ISString &o) const;

		/** Addition operator */
		ISString operator + (const ISString &o) const;

		/** */
		ISString &operator += (const ISString &o);

		/** Returns true if the left operand is considered greater than the right operand, otherwise returns false. */
		bool operator > (const ISString &o) const;

		/** Returns true if the left operand is considered smaller than the right operand, otherwise returns false. */
		bool operator < (const ISString &o) const;

		/** Returns true if the left operand is considered smaller than or equal to the right operand, otherwise returns false. */
		bool operator >= (const ISString &o) const;

		/** Returns true if the left operand is considered smaller than or equal to the right operand, otherwise returns false. */
		bool operator <= (const ISString &o) const;

		/** Returns the number of characters in the string. */
		size_t length() const;

		/** Returns the size of the string in bytes. */
		size_t size() const;

		/** Converts an ISString to the std::string */
		string toStdString() const;

		/** Returns char* pointer to the data in this string.
		  * This pointer will only be valid while the underlying string is not modified and not destroyed. */
		const char *cString() const;

		/** Converts an ISString to the int32_t. */
		int32_t toInt() const;

		/** Converts an ISString to the float. */
		float toFloat() const;

		/** Converts an ISString to the uint8_t. */
		uint8_t toByte() const;

		/** Converts an ISString to the uint16_t. */
		uint16_t toShort() const;


		/** Returns first n characters from this string. */
		ISString left(int32_t n) const;

		/** Retuns n characters from the end of this string. */
		ISString right(int32_t n) const;

		/** Returns n characters beginning from f's character */
		ISString mid(int32_t f, int32_t n = 0) const;

		/** Replaces all occurences of find with repl and returns result. */
		ISString replace(const ISString &find, const ISString &repl) const;

		/** Find the first occurrence of string needle in this string, beginning from startPos (inclusive).
		  * Returns position of the first character of the occurrence if it is found, otherwise returns 0. */
		int32_t inStr(const ISString &needle, int32_t startPos = 1) const;

		/** Returns this string converted to uppercase*/
		ISString toUpper() const;

		/** Returns this string converted to lowercase*/
		ISString toLower() const;

		/** Removes whitespace characters from the begin and the end of string and returns the result. */
		ISString trim() const;


		/** Detach this IString from shared-data. Copies data if required. */
		void detach();

		/** Get a pointer to ALLEGRO_PATH from the string inside */
		ALLEGRO_PATH* getPath() const;

		/** Returns true if the ISString is a null string */
		bool null() const {return data == 0; }

		/** Returns copy of the ISString data. Caller has to free ALLEGRO_USTR. */
		ALLEGRO_USTR *getUStr() const;

		/** Returns implicit-shared ALLEGRO_USTR that may become invalid if ISString is destroyed. */
		const ALLEGRO_USTR *getRef() const;

		/** Constructs an ISString from an int32_t */
		static ISString fromInt(int32_t n);

		/** Constructs an ISString from a float */
		static ISString fromFloat(float n);

		/** Constructs an ISString from a buffer */
		static ISString fromBuffer(const char *buf, size_t size);
	private:
		/** Implicit-shared data */
		class SharedData {
			public:
				SharedData(ALLEGRO_USTR *s): str(s), refCounter(1) {}
				~SharedData() { al_ustr_free(str); }
				void increaseRefCount();
				void decreaseRefCount();
				SharedData *detach();

				ALLEGRO_USTR *str;
				uint32_t refCounter;
		};
		/** Helper for converting a std::string to the ALLEGRO_USTR. */
		static ALLEGRO_USTR *createUStr(const string &s);

		/** Helper for converting a c-string to the ALLEGRO_USTR. */
		static ALLEGRO_USTR *createUStr(const char *s);

		/** Private constructor for implicit use. */
		ISString(SharedData *d):data(d) {if (d) d->increaseRefCount();}

		/** Shared data */
		SharedData *data;
};



#endif // ISSTRING_H
