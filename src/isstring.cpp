#include "isstring.h"

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
