#include "isstring.h"
#include <allegro5/allegro.h>

ISString::ISString(const char *str): data(0) {
	if (str && str[0] != '\0') {
		data = new SharedData(string(str));
	}
}

ISString::ISString(const string &str): data(0){
	if (str.length() != 0) data = new SharedData(str);
}

ISString::ISString(const ISString &str): data(str.data) {
	if (data != 0) {
		++data->refCounter;
		return;
	}
}

ISString::~ISString() {
	if (data) data->decreaseRefCount();
}

ISString & ISString::operator=(const ISString &o) {
	if (data != 0) data->decreaseRefCount();
	data = o.data;
	if (data != 0) data->increaseRefCount();
	return *this;
}

string ISString::getStdString()const{
	if (data == 0) return "";
	return data->str;
}

const string &ISString::getRef()const {
	static string nullStdString;
	if (data == 0) return nullStdString;
	return data->str;
}

bool ISString::operator==(const ISString &o) const {
	if (this->data == 0) {
		if (o.data == 0) return true;
		return false;
	}
	if (o.data == 0) return false;
	return o.data->str == data->str;
}

bool ISString::operator!=(const ISString &o) const {
	if (this->data == 0) {
		if (o.data == 0) return false;
		return true;
	}
	if (o.data == 0) return true;
	return o.data->str != data->str;
}

ISString ISString::operator+(const ISString &o) const {
	if (o.data == 0) {
		return *this;
	}
	if (data == 0) {
		return o;
	}
	return ISString(data->str+o.data->str);
}

int32_t ISString::toInt() const {
	if (data == 0) return 0;
	try {
		return (boost::lexical_cast<int32_t>(data->str));
	}
	catch (boost::bad_lexical_cast &) {
		return 0;
	}
}

float ISString::toFloat() const {
	if (data == 0) return 0.0f;
	try {
		return (boost::lexical_cast<float>(data->str));
	}
	catch (boost::bad_lexical_cast &) {
		return 0.0f;
	}
}

uint8_t ISString::toByte() const {
	if (data == 0) return 0;
	try {
		return (boost::lexical_cast<uint8_t>(data->str));
	}
	catch (boost::bad_lexical_cast &) {
		return 0;
	}
}

uint16_t ISString::toShort()const{
	if (data == 0) return 0;
	try {
		return (boost::lexical_cast<uint16_t>(data->str));
	}
	catch (boost::bad_lexical_cast &) {
		return 0;
	}
}

ISString ISString::add(const string&a,const ISString &b) {
	if (b.data == 0) return a;
	return a+b.data->str;
}

ISString ISString::operator+(const string &o) const{
	if (data == 0) {
		return o;
	}
	return ISString(data->str+o);
}

bool ISString::operator >(const ISString &o) const {
	if (this->data == 0) {
		return false;
	}
	if (o.data == 0) {
		if (o.data->str == "") {
			return false;
		}
		return true;
	}
	if (this->data->str.compare(o.data->str) > 0) {
		return true;
	}
	return false;
}

bool ISString::operator <(const ISString &o) const {
	if (o.data == 0) {
		return false;
	}
	if (this->data == 0) {
		if (this->data->str == "") {
			return false;
		}
		return true;
	}
	if (this->data->str.compare(o.data->str) < 0) {
		return true;
	}
	return false;
}

bool ISString::operator >=(const ISString &o) const {
	if (this->data == 0) {
		if (o.data == 0 || o.data->str == "") return true;
		return false;
	}
	if (o.data == 0) {
		if (o.data->str == "" && this->data->str != "") {
			return false;
		}
		return true;
	}
	if (this->data->str.compare(o.data->str) >= 0) {
		return true;
	}
	return false;
}

bool ISString::operator <=(const ISString &o) const {
	if (o.data == 0) {
		if (this->data == 0 || this->data->str == "") return true;
		return false;
	}
	if (this->data == 0) {
		if (this->data->str == "" && o.data->str != "") {
			return false;
		}
		return true;
	}
	if (this->data->str.compare(o.data->str) >= 0) {
		return true;
	}
	return false;
}

/** Get a pointer to ALLEGRO_PATH from the string inside */
ALLEGRO_PATH* ISString::getPath() const {
	if (data == 0) return al_create_path(NULL);
	return al_create_path(data->str.c_str());
}
