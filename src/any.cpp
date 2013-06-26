#include "any.h"
#include "variablestack.h"
#include "cbenchanted.h"
#include <cmath>
#include "errorsystem.h"

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
	if (a.typeId == String && a.dString) {
		a.dString->increaseRefCount();
	}
	if (this->typeId == String && this->dString) {
		this->dString->decreaseRefCount();
	}
	this->typeId = a.typeId;
	this->dPtr = a.dPtr;
	return *this;
}

const type_info &Any::typeInfo() const {
	switch (type()) {
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
		switch (type()) {
			case Any::String:
				return ISString(dString);
			case Any::Float: {
				ISString i(boost::lexical_cast<string>(getFloat()));
				i.requireEncoding(false);
				return i;
			}
			case Any::Int: {
				ISString i(boost::lexical_cast<string>(getInt()));
				i.requireEncoding(false);
				return i;
			}
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
	switch (type()) {
		case Any::Int:
			return getInt();
		case Any::Float:
			return ((int32_t)(getFloat() + 0.5f));
		case Any::String:
			if (dString == 0) return 0;
			try {
				std::string tmp = dString->str;
				boost::algorithm::trim(tmp);
				return boost::lexical_cast<int32_t>(tmp);
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
	switch (type()) {
		case Any::Float:
			return getFloat();
		case Any::Int:
			return ((float)getInt());
		case Any::String:
			if (dString == 0) return 0;
			try {
				std::string tmp = dString->str;
				boost::algorithm::trim(tmp);
				return boost::lexical_cast<float>(tmp);
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
	switch (type()) {
		case Any::Float:
			return ((uint16_t)getFloat());
		case Any::Int:
			return ((uint16_t)getInt());
		case Any::String:
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
	switch (type()) {
		case Any::Float:
			return ((uint8_t)getFloat());
		case Any::Int:
			return ((uint8_t)getInt());
		case Any::String:
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

/** Returns typeptr or NULL if value type is int and value is 0.
  * @return Converted typeptr or null
  */
void *Any::toTypePtr() const{
	if (typeId == TypePtr) {
		return dPtr;
	}
	if (typeId == Int) {
		if (dInt == 0) {
			return 0;
		}
		else {
			CBEnchanted::instance()->errors->createError(
						"Can't convert integer '"+boost::lexical_cast<string>(dInt)+"' to typepointer",
						"Only integer 0 can be converted to typepointer (NULL)");
		}
	}
	FIXME("Unsupported cast %s => type pointer", typeInfo().name());
	return 0;
}

/** Prints debug information to stdout.
 */
void Any::dump() const {
	switch (typeId) {
		case Int:
			printf("Integer %i\n", this->getInt()); return;
		case Float:
			printf("Float %f\n",this->getFloat()); return;
		case String:
			printf("String %s\n", this->getString().getRef().c_str()); return;
		case TypePtr:
			printf("TypePointer %X\n", (intptr_t)this->getTypePtr()); return;
		case Empty:
			printf("Invalid Any\n"); return;

	}
}

int32_t Any::operator ! () const {
	switch (this->type()) {
		case Any::Float:
			return !this->getFloat();
		case Any::Int:
			return !this->getInt();
		case Any::TypePtr:
			return !this->getTypePtr();
	}
	FIXME("Unsupported operation !%s", this->typeInfo().name());
	return 0;
}

Any Any::operator + () const {
	switch (this->type()) {
		case Any::Float:
			return abs(this->getFloat());
		case Any::Int:
			return abs(this->getInt());
	}
	FIXME("Unsupported operation +%s", this->typeInfo().name());
	return 0;
}

Any Any::operator - () const {
	switch (this->type()) {
		case Any::Float:
			return -this->getFloat();
		case Any::Int:
			return -this->getInt();
	}
	FIXME("Unsupported operation -%s", this->typeInfo().name());
	return 0;
}

Any Any::operator % (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return (float)fmod(this->getFloat(), r.getFloat());
				case Any::Int:
					return (float)fmod((double)this->getFloat(), (double)r.getInt());
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return (float)fmod((double)this->getInt(), (double)r.getFloat());
				case Any::Int:
					return this->getInt() % r.getInt();
			}
			break;
	}
	FIXME("Unsupported operation %s % %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any Any::operator * (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return this->getFloat() * r.getFloat();
				case Any::Int:
					return this->getFloat() * r.getInt();
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return this->getInt() * r.getFloat();
				case Any::Int:
					return this->getInt() * r.getInt();
			}
			break;
	}
	FIXME("Unsupported operation %s * %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any Any::operator + (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return this->getFloat() + r.getFloat();
				case Any::Int:
					return this->getFloat() + r.getInt();
				case Any::String:
					return boost::lexical_cast<string>(this->getFloat()) + r.getString();
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return this->getInt() + r.getFloat();
				case Any::Int:
					return this->getInt() + r.getInt();
				case Any::String:
					return boost::lexical_cast<string>(this->getInt()) + r.getString();
			}
			break;
		case Any::String:
			switch (r.type()) {
				case Any::Float:
					return this->getString() + boost::lexical_cast<string>(r.getFloat());
				case Any::Int:
					return this->getString() + boost::lexical_cast<string>(r.getInt());
				case Any::String:
					return this->getString() + r.getString();
			}
			break;
	}
	FIXME("Unsupported operation %s + %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any Any::operator - (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return this->getFloat() - r.getFloat();
				case Any::Int:
					return this->getFloat() - r.getInt();
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return this->getInt() - r.getFloat();
				case Any::Int:
					return this->getInt() - r.getInt();
			}
			break;
	}
	FIXME("Unsupported operation %s - %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any Any::operator / (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return this->getFloat() / r.getFloat();
				case Any::Int:
					return this->getFloat() / r.getInt();
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return this->getInt() / r.getFloat();
				case Any::Int:
					return this->getInt() / r.getInt();
			}
			break;
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
}

int32_t Any::shr (const Any &l, const Any &r) {
	if (l.type() == Any::Int && r.type() == Any::Int) {
		int32_t a = l.getInt();
		int32_t b = r.getInt();
		uint32_t ret = *reinterpret_cast<uint32_t*>( &a ) >> *reinterpret_cast<uint32_t*>( &b );
		return *reinterpret_cast<int32_t*>( &ret );
	}
	FIXME("Unsupported operation %s >> %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any Any::operator ^ (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return (float)pow(this->getFloat(), r.getFloat());
				case Any::Int:
					return (float)pow(this->getFloat(), r.getInt());
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return (int)pow((double)this->getInt(), (double)r.getFloat());
				case Any::Int:
					return (int)pow((double)this->getInt(), r.getInt());
			}
			break;
	}
	FIXME("Unsupported operation %s ^ %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator != (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return this->getFloat() != r.getFloat();
				case Any::Int:
					return this->getFloat() != r.getInt();
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return this->getInt() != r.getFloat();
				case Any::Int:
					return this->getInt() != r.getInt();
			}
			break;
		case Any::String:
			if (r.type() == Any::String) {
				return this->getString() != r.getString();
			}
			break;
		case Any::TypePtr:
			if (r.type() == Any::TypePtr) {
				return r.getTypePtr() != this->getTypePtr();
			}
			break;
	}
	FIXME("Unsupported operation %s != %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator && (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return this->getFloat() && r.getFloat();
				case Any::Int:
					return this->getFloat() && r.getInt();
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return this->getInt() && r.getFloat();
				case Any::Int:
					return this->getInt() && r.getInt();
			}
			break;
		case Any::TypePtr:
			if (r.type() == Any::TypePtr) {
				return r.getTypePtr() && this->getTypePtr();
			}
			break;
	}
	FIXME("Unsupported operation %s && %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator <= (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return this->getFloat() <= r.getFloat();
				case Any::Int:
					return this->getFloat() <= r.getInt();
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return this->getInt() <= r.getFloat();
				case Any::Int:
					return this->getInt() <= r.getInt();
			}
			break;
	}
	FIXME("Unsupported operation %s <= %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator == (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return this->getFloat() == r.getFloat();
				case Any::Int:
					return this->getFloat() == r.getInt();
				case Any::String:
					try {
						return this->getFloat() == boost::lexical_cast<float>(r.getString().getRef());
					}
					catch( boost::bad_lexical_cast &) {
						return 0;
					}
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return this->getInt() == r.getFloat();
				case Any::Int:
					return this->getInt() == r.getInt();
				case Any::String:
					try {
						return this->getInt() == boost::lexical_cast<int32_t>(r.getString().getRef());
					}
					catch( boost::bad_lexical_cast &) {
						return 0;
					}
			}
			break;
		case Any::String:
			switch (r.type()) {
				case Any::String:
					return this->getString() == r.getString();
				case Any::Int:
					try {
						return boost::lexical_cast<int32_t>(this->getString().getRef()) == r.getInt();
					}
					catch( boost::bad_lexical_cast &) {
						return 0;
					}
				case Any::Float:
					try {
						return boost::lexical_cast<float>(this->getString().getRef()) == r.getFloat();
					}
					catch( boost::bad_lexical_cast &) {
						return 0;
					}
			}
			break;
		case Any::TypePtr:
			if (r.type() == Any::TypePtr) {
				return r.getTypePtr() == this->getTypePtr();
			}
			break;
	}
	FIXME("Unsupported operation %s == %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator >= (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return this->getFloat() >= r.getFloat();
				case Any::Int:
					return this->getFloat() >= r.getInt();
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return this->getInt() >= r.getFloat();
				case Any::Int:
					return this->getInt() >= r.getInt();
			}
			break;
	}
	FIXME("Unsupported operation %s >= %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator || (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return this->getFloat() || r.getFloat();
				case Any::Int:
					return this->getFloat() || r.getInt();
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return this->getInt() || r.getFloat();
				case Any::Int:
					return this->getInt() || r.getInt();
			}
			break;
		case Any::TypePtr:
			if (r.type() == Any::TypePtr) {
				return r.getTypePtr() || this->getTypePtr();
			}
			break;
	}
	FIXME("Unsupported operation %s || %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator > (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return this->getFloat() > r.getFloat();
				case Any::Int:
					return this->getFloat() > r.getInt();
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return this->getInt() > r.getFloat();
				case Any::Int:
					return this->getInt() > r.getInt();
			}
			break;
	}
	FIXME("Unsupported operation %s >= %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t Any::operator < (const Any &r) const {
	switch (this->type()) {
		case Any::Float:
			switch (r.type()) {
				case Any::Float:
					return this->getFloat() < r.getFloat();
				case Any::Int:
					return this->getFloat() < r.getInt();
			}
			break;
		case Any::Int:
			switch (r.type()) {
				case Any::Float:
					return this->getInt() < r.getFloat();
				case Any::Int:
					return this->getInt() < r.getInt();
			}
			break;
	}
	FIXME("Unsupported operation %s >= %s", this->typeInfo().name(), r.typeInfo().name());
	return 0;
}
