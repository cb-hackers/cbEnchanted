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

		ISString() : data(0){ }
		ISString(const char *str);

		ISString(const string &str);

		ISString(const ISString &str);

		~ISString();

		ISString & operator=(const ISString &o);

		string getStdString()const;

		const string &getRef()const;

		bool operator==(const ISString &o) const;

		bool operator!=(const ISString &o) const;

		ISString operator+(const ISString &o) const;

		ISString operator+(const string &o) const;

		bool operator > (const ISString &o) const;
		bool operator < (const ISString &o) const;
		bool operator >= (const ISString &o) const;
		bool operator <= (const ISString &o) const;

		static ISString add(const string&a,const ISString &b);

		int32_t toInt() const;
		float toFloat() const;
		uint8_t toByte() const;
		uint16_t toShort() const;
		static ISString nullString;

		/** Get a pointer to ALLEGRO_PATH from the string inside */
		ALLEGRO_PATH* getPath() const;
	private:
		/** Implicit-shared data */
		class SharedData {
			public:
				SharedData():str(),refCounter(1){}
				SharedData(const string &s):str(s),refCounter(1){}
				string str;
				uint32_t refCounter;
				void increaseRefCount() {++refCounter;}
				void decreaseRefCount() { if (--refCounter == 0) delete this; }
		};
		ISString(SharedData *d):data(d){if (data) data->increaseRefCount();}
		SharedData *data;
};
inline ISString operator +(const string &a,const ISString&b) {
	return ISString::add(a,b);
}




#endif // ISSTRING_H
