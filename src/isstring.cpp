#include "isstring.h"
#include <allegro5/allegro.h>
#include <string>

inline void ISString::SharedData::increaseRefCount() {
	this->refCounter++;
}

inline void ISString::SharedData::decreaseRefCount() {
	if ((--this->refCounter) == 0) {
		delete this;
	}
}

inline ISString::SharedData *ISString::SharedData::detach() {
	if (refCounter == 1) return this;
	refCounter--;
	return new SharedData(al_ustr_dup(this->str));
}

ALLEGRO_USTR *ISString::createUStr(const string &s) {
	string result;
	result.reserve(s.size()+s.size()/4);
	for (string::const_iterator i = s.begin(); i != s.end(); i++) {
		unsigned char c = *i;
		if (c > 128) {
			c--;
			char utfc[2];
			al_utf8_encode(utfc, c);
			result += utfc[0];
			result += utfc[1];
		}
		else {
			result += c;
		}
	}
	return al_ustr_new(result.c_str());
}

ALLEGRO_USTR *ISString::createUStr(const char *s) {
	string result;
	unsigned char *c = (unsigned char*)s;
	while (*c) {
		if (*c > 128) {
			*c--;
			char utfc[2];
			al_utf8_encode(utfc, *c);
			result += utfc[0];
			result += utfc[1];
		}
		else {
			result += *c;
		}
		c++;
	}
	return al_ustr_new(result.c_str());
}


ISString::ISString(const char *str): data(0) {
	if (str && str[0] != 0) {
		data = new SharedData(createUStr(str));
	}
}

ISString::ISString(const string &str): data(0){
	if (str.length() != 0) {
		data = new SharedData(createUStr(str));
	}
}

ISString::ISString(ALLEGRO_USTR *str): data(0) {
	if (str == al_ustr_empty_string()) {
		return;
	}
	data = new SharedData(str);
}

ISString::ISString(const ISString &str): data(str.data) {
	if (data != 0) {
		data->increaseRefCount();
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

string ISString::toStdString() const {
	if (data == 0) return string();
	return string(al_cstr(data->str));
}

const char *ISString::cString() const {
	if (data == 0) return al_cstr(al_ustr_empty_string());
	return al_cstr(data->str);
}

ALLEGRO_USTR *ISString::getUStr() const {
	if (data == 0) return al_ustr_dup(al_ustr_empty_string());
	return al_ustr_dup(data->str);
}

const ALLEGRO_USTR *ISString::getRef() const {
	if (data == 0) return al_ustr_empty_string();
	return data->str;
}

ISString ISString::fromInt(int32_t n) {
	return ISString(al_ustr_new(boost::lexical_cast<string>(n).c_str()));
}

ISString ISString::fromFloat(float n) {
	return ISString(al_ustr_new(boost::lexical_cast<string>(n).c_str()));
}

ISString ISString::fromBuffer(const char *buf, size_t size) {
	return ISString(al_ustr_new_from_buffer(buf,size));
}

bool ISString::operator==(const ISString &o) const {
	if (this->data == 0) {
		if (o.data == 0) return true;
		return false;
	}
	if (o.data == 0) return false;
	return al_ustr_equal(o.data->str, data->str);
}

bool ISString::operator!=(const ISString &o) const {
	if (this->data == 0) {
		if (o.data == 0) return false;
		return true;
	}
	if (o.data == 0) return true;
	return !al_ustr_equal(o.data->str, data->str);
}

ISString ISString::operator+(const ISString &o) const {
	if (o.data == 0) {
		return *this;
	}
	if (data == 0) {
		return o;
	}
	ALLEGRO_USTR *result = al_ustr_dup(this->data->str);
	assert(al_ustr_append(result, o.data->str));
	return ISString(result);
}

ISString &ISString::operator +=(const ISString &o) {
	if (o.data == 0) return *this;
	if (this->data == 0) {
		this->data = o.data;
		if (this->data) {
			this->data->increaseRefCount();
		}
		return *this;
	}
	this->data->detach();
	assert(al_ustr_append(this->data->str, o.data->str));
	return *this;
}

int32_t ISString::toInt() const {
	if (data == 0) return 0;
	try {
		return stoi(al_cstr(data->str));
	}
	catch (std::exception &) {
	}
	return 0;
}

float ISString::toFloat() const {
	if (data == 0) return 0.0f;
	try {
		return stof(al_cstr(data->str));
	}
	catch (std::exception &) {
	}
	return 0;
}

uint8_t ISString::toByte() const {
	if (data == 0) return 0;
	try {
		return static_cast<uint8_t>(stoi(al_cstr(data->str)));
	}
	catch (std::exception &) {
	}
	return 0;
}

uint16_t ISString::toShort()const{
	if (data == 0) return 0;
	try {
		return static_cast<uint16_t>(stoi(al_cstr(data->str)));
	}
	catch (std::exception &) {
	}
	return 0;
}

void ISString::detach() {
	if (data) {

	}
}

bool ISString::operator >(const ISString &o) const {
	if (this->data == 0) {
		return false;
	}
	if (o.data == 0) {
		return true;
	}
	if (al_ustr_compare(this->data->str, o.data->str) > 0) {
		return true;
	}
	return false;
}

bool ISString::operator <(const ISString &o) const {
	if (o.data == 0) {
		return false;
	}
	if (this->data == 0) {
		return true;
	}
	if (al_ustr_compare(this->data->str, o.data->str) < 0) {
		return true;
	}
	return false;
}

bool ISString::operator >=(const ISString &o) const {
	if (o.data == 0) {
		return true;
	}
	if (this->data == 0) {
		return false;
	}
	if (al_ustr_compare(this->data->str, o.data->str) >= 0) {
		return true;
	}
	return false;
}

bool ISString::operator <=(const ISString &o) const {
	if (this->data == 0) {
		return true;
	}
	if (o.data == 0) {
		return false;
	}
	if (al_ustr_compare(this->data->str, o.data->str) <= 0) {
		return true;
	}
	return false;
}

size_t ISString::length() const {
	if (this->data == 0) return 0;
	return al_ustr_length(this->data->str);
}

size_t ISString::size() const {
	if (this->data == 0) return 0;
	return al_ustr_size(this->data->str);
}

/** Get a pointer to ALLEGRO_PATH from the string inside */
ALLEGRO_PATH* ISString::getPath() const {
	if (data == 0) return al_create_path(NULL);
	return al_create_path(al_cstr(data->str));
}


ISString ISString::left(int32_t n) const {
	if (this->data == 0) return ISString();
	return ISString(
				al_ustr_dup_substr(this->data->str, al_ustr_offset(this->data->str, 0), al_ustr_offset(this->data->str, n - 1)));
}

ISString ISString::right(int32_t n) const {
	if (this->data == 0) return ISString();
	return ISString(
				al_ustr_dup_substr(this->data->str, al_ustr_offset(this->data->str, -n), size()));
}

ISString ISString::mid(int32_t f, int32_t n) const {
	if (this->data == 0) return ISString();
	if (n == -1) {
		return ISString(
					al_ustr_dup_substr(this->data->str, al_ustr_offset(this->data->str, f - 1), size()));
	}
	return ISString(
				al_ustr_dup_substr(this->data->str, al_ustr_offset(this->data->str, f - 1),al_ustr_offset(this->data->str, f + n - 1)));
}

ISString ISString::toUpper() const {
	//TODO
	return *this;
}

ISString ISString::toLower() const {
	//TODO
	return *this;
}

ISString ISString::trim() const {
	if (this->data == 0) return ISString();
	ALLEGRO_USTR *result = al_ustr_dup(this->data->str);
	assert(al_ustr_trim_ws(result));
	return ISString(result);
}

int32_t ISString::inStr(const ISString &needle, int32_t startPos) const {
	if (this->data == 0 || needle.data == 0) return 0;
	return 1 + al_ustr_find_str(this->data->str, al_ustr_offset(this->data->str, startPos), needle.data->str);
}

ISString ISString::replace(const ISString &find, const ISString &repl) const {
	if (this->data == 0) return ISString();
	if (find.data == 0) return *this;
	ALLEGRO_USTR *r = repl.data->str;
	if (r == 0) r = al_ustr_empty_string();

	ALLEGRO_USTR *result = al_ustr_dup(this->data->str);

	assert(al_ustr_find_replace(result, 0, find.data->str, r));
	return ISString(result);
}




