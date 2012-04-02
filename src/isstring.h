#ifndef ISSTRING_H
#define ISSTRING_H
#include "precomp.h"
class Any;
/*
  * Implicit-shared string
  */

class ISString {
	public:
		friend class Any;

		ISString() : data(0){

		}
		ISString(const char *str):data(0){if (str[0] != '\0') data = new SharedData(string(str));}
		ISString(const string &str):data(0){
			if (str.length() != 0) data = new SharedData(str);
		}

		ISString(const ISString &str):data(str.data) {
			if (data != 0) {
				++data->refCounter;
				return;
			}
		}
		~ISString() {
			if (data) data->decreaseRefCount();
		}
		ISString & operator=(const ISString &o) {
			if (data != 0) data->decreaseRefCount();
			data = o.data;
			if (data != 0) data->increaseRefCount();
			return *this;
		}
		string getStdString()const{
			if (data == 0) return "";
			return data->str;
		}
		const string &getRef()const{
			static string nullStdString;
			if (data == 0) return nullStdString;
			return data->str;
		}

		bool operator==(const ISString &o) {
			if (this->data == 0) {
				if (o.data == 0) return true;
				return false;
			}
			if (o.data == 0) return false;
			return o.data->str == data->str;
		}
		bool operator!=(const ISString &o) {
			if (this->data == 0) {
				if (o.data == 0) return false;
				return true;
			}
			if (o.data == 0) return true;
			return o.data->str != data->str;
		}
		ISString operator+(const ISString &o) const{
			if (o.data == 0) {
				return *this;
			}
			if (data == 0) {
				return o;
			}
			return ISString(data->str+o.data->str);
		}
		ISString operator+(const string &o) const{
			if (data == 0) {
				return o;
			}
			return ISString(data->str+o);
		}
		static ISString add(const string&a,const ISString &b) {
			if (b.data == 0) return a;
			return a+b.data->str;
		}

		ISString operator +(const string & o) {
			if (this->data == 0) return o;
			return ISString(data->str + o);
		}

		int32_t toInt()const{
			if (data == 0) return 0;
			try {
				return (boost::lexical_cast<int32_t>(data->str));
			}
			catch (boost::bad_lexical_cast &) {
				return 0;
			}
		}
		float toFloat()const{
			if (data == 0) return 0.0f;
			try {
				return (boost::lexical_cast<float>(data->str));
			}
			catch (boost::bad_lexical_cast &) {
				return 0.0f;
			}
		}
		uint8_t toByte()const{
			if (data == 0) return 0;
			try {
				return (boost::lexical_cast<uint8_t>(data->str));
			}
			catch (boost::bad_lexical_cast &) {
				return 0;
			}
		}
		uint16_t toShort()const{
			if (data == 0) return 0;
			try {
				return (boost::lexical_cast<uint16_t>(data->str));
			}
			catch (boost::bad_lexical_cast &) {
				return 0;
			}
		}
		static ISString nullString;
		/*static ISString operator +(const string &a,const ISString &b) {
		if (b.data == 0) return a;
		return a+b.data->str;
	}*/
	private:
		class SharedData {
			public:
				SharedData():str(),refCounter(1){}
				SharedData(const string &s):str(s),refCounter(1){}
				string str;
				uint32_t refCounter;
				void increaseRefCount() {++refCounter;}
				void decreaseRefCount() {
					if (--refCounter == 0) delete this;
				}
		};
		ISString(SharedData *d):data(d){if (data) data->increaseRefCount();}
		SharedData *data;
};
inline ISString operator +(const string &a,const ISString&b) { return ISString::add(a,b);}

#endif // ISSTRING_H
