#include "any.h"
#include "variablestack.h"
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

Any::Any(const ISString &a) : typeId(String), dString(a.data) {
	if (dString != 0) {
		++dString->refCounter;
		return;
	}
}

Any::Any(const Any &a) : typeId(a.typeId) {
	if (a.typeId == String) {
		this->dString = a.dString;
		if (this->dString) {
			this->dString->increaseRefCount();
		}
		return;
	}
	this->dPtr = a.dPtr;
}

Any::~Any() {
	if (typeId == String && dString) {
		dString->decreaseRefCount();
	}
}

Any &Any::operator =(const Any &a) {
	if (a.typeId == String) {
		if (a.dString) a.dString->increaseRefCount();
	}
	if (this->typeId == String) {
		if (this->dString) this->dString->decreaseRefCount();
	}
	this->typeId = a.typeId;
	this->dPtr = a.dPtr;
	return *this;
}

const type_info &Any::typeInfo() const {
	switch (typeId) {
		case Int: return typeid(int32_t);
		case Float: return typeid(float);
		case String: return typeid(ISString);
		case TypePtr: return typeid(void*);
		default: return typeid(void);
	}
}


ISString Any::toString() const {
	assert(!empty());
	try {
		if (type() == Any::String) {
			return ISString(dString);
		}
		if (type() == Any::Float) {
			ISString i(boost::lexical_cast<string>(getFloat()));
			i.requireEncoding(false);
			return i;
		}
		if (type() == Any::Int) {
			ISString i(boost::lexical_cast<string>(getInt()));
			i.requireEncoding(false);
			return i;
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
		return ((int32_t)(getFloat()+0.5f));
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






int32_t Any::operator ! () const {
	if (this->type() == Any::Float) {
		return !this->getFloat();
	}
	if (this->type() == Any::Int) {
		return !this->getInt();
	}
	if (this->type() == Any::TypePtr) {
		return !this->getTypePtr();
	}
	FIXME("Unsupported operation !%s", this->typeInfo().name());
	return 0;
}

Any Any::operator + () const {
	if (this->type() == Any::Float) {
		return +this->getFloat();
	}
	if (this->type() == Any::Int) {
		return +this->getInt();
	}
	FIXME("Unsupported operation +%s", this->typeInfo().name());
	return 0;
}

Any Any::operator - () const {
	if (this->type() == Any::Float) {
		return -this->getFloat();
	}
	if (this->type() == Any::Int) {
		return -this->getInt();
	}
	FIXME("Unsupported operation -%s", this->typeInfo().name());
	return 0;
}

Any Any::operator % (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return (float)fmod(this->getFloat(), r.getFloat());
		}
		if (r.type() == Any::Int) {
			return (float)fmod((double)this->getFloat(), (double)r.getInt());
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return (float)fmod((double)this->getInt(), (double)r.getFloat());
		}
		if (r.type() == Any::Int) {
			return this->getInt() % r.getInt();
		}
	}
	FIXME("Unsupported operation %s % %s", this->typeInfo().name(), r.typeInfo().name());
}

Any Any::operator * (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return this->getFloat() * r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getFloat() * r.getInt();
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return this->getInt() * r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getInt() * r.getInt();
		}
	}
	FIXME("Unsupported operation %s * %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any Any::operator + (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return this->getFloat() + r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getFloat() + r.getInt();
		}
		if (r.type() == Any::String) {
			return boost::lexical_cast<string>(this->getFloat()) + r.getString();
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return this->getInt() + r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getInt() + r.getInt();
		}
		if (r.type() == Any::String) {
			return boost::lexical_cast<string>(this->getInt()) + r.getString();
		}
	}
	if (this->type() == Any::String) {
		if (r.type() == Any::Float) {
			return this->getString() + boost::lexical_cast<string>(r.getFloat());
		}
		if (r.type() == Any::Int) {
			return this->getString() + boost::lexical_cast<string>(r.getInt());
		}
		if (r.type() == Any::String) {
			return this->getString() + r.getString();
		}
	}
	FIXME("Unsupported operation %s + %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any Any::operator - (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return this->getFloat() - r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getFloat() - r.getInt();
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return this->getInt() - r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getInt() - r.getInt();
		}
	}
	FIXME("Unsupported operation %s - %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any Any::operator / (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return this->getFloat() / r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getFloat() / r.getInt();
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return this->getInt() / r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getInt() / r.getInt();
		}
	}
	FIXME("Unsupported operation %s / %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any Any::operator << (const Any &r) const {
	if (this->type() == Any::Int) {
		if (r.type() == Any::Int) {
			int32_t a = this->getInt();
			int32_t b = r.getInt();
			uint32_t ret = *reinterpret_cast<uint32_t*>( &a ) << *reinterpret_cast<uint32_t*>( &b );
			return *reinterpret_cast<int32_t*>( &ret );
		}
	}
	FIXME("Unsupported operation %s << %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::sar (const Any &l, const Any &r) {
	return l.toInt() >> r.toInt();
	return 0;
}

int32_t Any::shr (const Any &l, const Any &r) {
	if (l.type() == Any::Int) {
		if (r.type() == Any::Int) {
			int32_t a = l.getInt();
			int32_t b = r.getInt();
			uint32_t ret = *reinterpret_cast<uint32_t*>( &a ) >> *reinterpret_cast<uint32_t*>( &b );
			return *reinterpret_cast<int32_t*>( &ret );
		}
	}
	FIXME("Unsupported operation %s >> %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any Any::operator ^ (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return (float)pow(this->getFloat(), r.getFloat());
		}
		if (r.type() == Any::Int) {
			return (float)pow(this->getFloat(), r.getInt());
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return (int)pow((double)this->getInt(), (double)r.getFloat());
		}
		if (r.type() == Any::Int) {
			return (int)pow((double)this->getInt(), r.getInt());
		}
	}
	FIXME("Unsupported operation %s ^ %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator != (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return this->getFloat() != r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getFloat() != r.getInt();
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return this->getInt() != r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getInt() != r.getInt();
		}
	}
	if (this->type() == Any::String) {
		if (r.type() == Any::String) {
			return this->getString() != r.getString();
		}
	}
	if (this->type() == Any::TypePtr) {
		if (r.type() == Any::TypePtr) {
			return r.getTypePtr() != this->getTypePtr();
		}
	}
	FIXME("Unsupported operation %s != %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator && (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return this->getFloat() && r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getFloat() && r.getInt();
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return this->getInt() && r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getInt() && r.getInt();
		}
	}
	if (this->type() == Any::TypePtr) {
		if (r.type() == Any::TypePtr) {
			return r.getTypePtr() && this->getTypePtr();
		}
	}
	FIXME("Unsupported operation %s && %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator <= (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return this->getFloat() <= r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getFloat() <= r.getInt();
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return this->getInt() <= r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getInt() <= r.getInt();
		}
	}
	FIXME("Unsupported operation %s <= %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator == (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return this->getFloat() == r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getFloat() == r.getInt();
		}
		if (r.type() == Any::String) {
			try {
				return this->getFloat() == boost::lexical_cast<float>(r.getString().getRef());
			}
			catch( boost::bad_lexical_cast &) {
				return 0;
			}
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return this->getInt() == r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getInt() == r.getInt();
		}
		if (r.type() == Any::String) {
			try {
				return this->getInt() == boost::lexical_cast<int32_t>(r.getString().getRef());
			}
			catch( boost::bad_lexical_cast &) {
				return 0;
			}
		}
	}
	if (this->type() == Any::String) {
		if (r.type() == Any::String) {
			return this->getString() == r.getString();
		}
		if (r.type() == Any::Int) {
			try {
				return boost::lexical_cast<int32_t>(this->getString().getRef()) == r.getInt();
			}
			catch( boost::bad_lexical_cast &) {
				return 0;
			}
		}
		if (r.type() == Any::Float) {
			try {
				return boost::lexical_cast<float>(this->getString().getRef()) == r.getFloat();
			}
			catch( boost::bad_lexical_cast &) {
				return 0;
			}
		}
	}
	if (this->type() == Any::TypePtr) {
		if (r.type() == Any::TypePtr) {
			return r.getTypePtr() == this->getTypePtr();
		}
	}
	FIXME("Unsupported operation %s == %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator >= (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return this->getFloat() >= r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getFloat() >= r.getInt();
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return this->getInt() >= r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getInt() >= r.getInt();
		}
	}
	FIXME("Unsupported operation %s >= %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator || (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return this->getFloat() || r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getFloat() || r.getInt();
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return this->getInt() || r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getInt() || r.getInt();
		}
	}
	if (this->type() == Any::TypePtr) {
		if (r.type() == Any::TypePtr) {
			return r.getTypePtr() || this->getTypePtr();
		}
	}
	FIXME("Unsupported operation %s || %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator > (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return this->getFloat() > r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getFloat() > r.getInt();
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return this->getInt() > r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getInt() > r.getInt();
		}
	}
	FIXME("Unsupported operation %s >= %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator < (const Any &r) const {
	if (this->type() == Any::Float) {
		if (r.type() == Any::Float) {
			return this->getFloat() < r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getFloat() < r.getInt();
		}
	}
	if (this->type() == Any::Int) {
		if (r.type() == Any::Float) {
			return this->getInt() < r.getFloat();
		}
		if (r.type() == Any::Int) {
			return this->getInt() < r.getInt();
		}
	}
	FIXME("Unsupported operation %s >= %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}





