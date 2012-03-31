#include "any.h"
Any::Any() : typeId(Empty) {}

Any::Any(float a) : typeId(Float), dFloat(a) { }

Any::Any(void *ptr) :typeId(TypePtr), dPtr(ptr) { }

Any::Any(int32_t a) : typeId(Int), dInt(a) { }

Any::Any(const string &a) : typeId(String) {
	if (a.length() != 0) {
		dString = new ISString::SharedData(a);
		return;
	}
	dString = 0;
}

Any::Any(const ISString &a):typeId(String),dString(a.data) {
	if (dString != 0) {
		++dString->refCounter;
		return;
	}
}

Any::Any(const Any &a) : typeId(a.typeId) {
	if (a.typeId == String) {
		this->dString = a.dString;
		if ( this->dString ) { this->dString->increaseRefCount(); }
		return;
	}
	dPtr = a.dPtr;
}

Any::~Any() {
	if ( typeId == String ) {
		if ( dString ) dString->decreaseRefCount();
	}
}

Any &Any::operator =(const Any &a)
{
	if ( a.typeId == String && a.dString ) { a.dString->increaseRefCount(); }
	if ( this->typeId == String && this->dString ) { this->dString->decreaseRefCount(); }
	this->typeId = a.typeId;
	this->dPtr = a.dPtr;
	return *this;
}

const type_info &Any::typeInfo() const{
	switch (typeId) {
		case Int:
			return typeid(int32_t);
		case Float:
			return typeid(float);
		case String:
			return typeid(ISString);
		default:
			return typeid(void);
	}
}


ISString Any::toString() const{
	assert(!empty());
	try {
		if (type() == Any::String) {
			return ISString(dString);
		}
		if (type() == Any::Float) {
			return ISString(boost::lexical_cast<string>(getFloat()));
		}
		if (type() == Any::Int) {
			return ISString(boost::lexical_cast<string>(getInt()));
		}
	}
	catch (boost::bad_lexical_cast &) {
		return ISString();
	}
	FIXME("Unsupported cast %s >= %s", typeInfo().name(), typeid(string).name());
	return ISString();
}

int32_t Any::toInt() const {
	assert(!empty());
	if (type() == Any::Int) {
		return getInt();
	}
	if (type() == Any::Float) {
		return ((int32_t)getFloat());
	}
	if (type() == Any::String) {
		if (dString == 0) return 0;
		try {
			return boost::lexical_cast<int32_t>(dString->str);
		}
		catch (boost::bad_lexical_cast &) {
			return 0;
		}
	}
	FIXME("Unsupported cast %s >= %s", typeInfo().name(), typeid(int32_t).name());
	return 0;
}

float Any::toFloat() const {
	assert(!empty());
	if (type() == Any::Float) {
		return getFloat();
	}
	if (type() == Any::Int) {
		return ((float)getInt());
	}
	if (type() == Any::String) {
		if (dString == 0) return 0;
		try {
			return boost::lexical_cast<float>(dString->str);
		}
		catch (boost::bad_lexical_cast &) {
			return 0;
		}
	}
	FIXME("Unsupported cast %s >= %s", typeInfo().name(), typeid(float).name());
	return 0.0f;
}

uint16_t Any::toShort() const {
	assert(!empty());
	if (type() == Any::Float) {
		return ((uint16_t)getFloat());
	}
	if (type() == Any::Int) {
		return ((uint16_t)getInt());
	}
	if (type() == Any::String) {
		if (dString == 0) return 0;
		try {
			return boost::lexical_cast<uint16_t>(dString->str);
		}
		catch (boost::bad_lexical_cast &) {
			return 0;
		}
	}
	FIXME("Unsupported cast %s >= %s", typeInfo().name(), typeid(uint16_t).name());
	return 0;
}

uint8_t Any::toByte() const {
	assert(!empty());
	if (type() == Any::Float) {
		return ((uint8_t)getFloat());
	}
	if (type() == Any::Int) {
		return ((uint8_t)getInt());
	}
	if (type() == Any::String) {
		if (dString == 0) return 0;
		try {
			return boost::lexical_cast<uint8_t>(dString->str);
		}
		catch (boost::bad_lexical_cast &) {
			return 0;
		}
	}
	FIXME("Unsupported cast %s >= %s", typeInfo().name(), typeid(uint8_t).name());
	return 0;
}
