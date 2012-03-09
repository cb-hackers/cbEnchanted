#ifndef ANY_H
#define ANY_H

#include "debug.h"
#include <assert.h>

class Any {
	public:
		enum Type {
			Empty = 0,
			Float = 1,
			Int = 2,
			String = 3
		};
	private:
		int32_t typeId;
		union { //Data
			float d_float;
			int32_t d_int;
			string *d_string_ptr;
			void *d_ptr;
		};

	public:
		inline Any() : typeId(Empty) {}
		inline Any(int32_t a) : typeId(Int), d_int(a) { }
		inline Any(float a) : typeId(Float), d_float(a) { }
		inline Any(const string &a) : typeId(String), d_string_ptr(new string(a)) { }
		inline Any(const Any &a) : typeId(a.typeId) {
			if (a.typeId == String) {
				d_string_ptr = new string(*a.d_string_ptr);
				return;
			}
			d_int = a.d_int; //Don't have to worry about data type.
		}
		inline ~Any() {
			if (typeId == String) {
				delete d_string_ptr;
			}
		}

		inline const string &getString() const { assert(typeId == String); return *d_string_ptr; }
		inline int32_t getInt() const { assert(typeId == Int); return d_int; }
		inline float getFloat() const { assert(typeId == Float); return d_float; }
		inline bool empty() const { return typeId == Empty; }
		inline int32_t type() const{ return typeId; }

		const type_info &typeInfo() const{
			switch (typeId) {
				case Int:
					return typeid(int32_t);
				case Float:
					return typeid(float);
				case String:
					return typeid(string);
				default:
					return typeid(void);
			}
		}

		inline string toString() const{
			assert(!empty());
			try {
				if (type() == Any::String) {
					return getString();
				}
				if (type() == Any::Float) {
					return boost::lexical_cast<string>(getFloat());
				}
				if (type() == Any::Int) {
					return boost::lexical_cast<string>(getInt());
				}
			}
            catch (boost::bad_lexical_cast &) {
				return "";
			}
			FIXME("Unsupported cast %s >= %s", typeInfo().name(), typeid(string).name());
			return "";
		}

		inline int32_t toInt() const {
			assert(!empty());
			if (type() == Any::Int) {
				return getInt();
			}
			if (type() == Any::Float) {
				return ((int32_t)getFloat());
			}
			if (type() == Any::String) {
				try {
					return (boost::lexical_cast<int32_t>(getString()));
				}
				catch (boost::bad_lexical_cast &error) {
					return 0;
				}
			}
			FIXME("Unsupported cast %s >= %s", typeInfo().name(), typeid(int32_t).name());
			return 0;
		}

		inline float toFloat() const {
			assert(!empty());
			if (type() == Any::Float) {
				return getFloat();
			}
			if (type() == Any::Int) {
				return ((float)getInt());
			}
			if (type() == Any::String) {
				try {
					return (boost::lexical_cast<float>(getString()));
				}
				catch (boost::bad_lexical_cast &error) {
					return 0.0f;
				}
			}
			FIXME("Unsupported cast %s >= %s", typeInfo().name(), typeid(float).name());
			return 0.0f;
		}

		inline uint16_t toShort() const {
			assert(!empty());
			if (type() == Any::Float) {
				return ((uint16_t)getFloat());
			}
			if (type() == Any::Int) {
				return ((uint16_t)getInt());
			}
			if (type() == Any::String) {
				try {
					return (boost::lexical_cast<uint16_t>(getString()));
				}
				catch (boost::bad_lexical_cast &error) {
					return 0;
				}
			}
			FIXME("Unsupported cast %s >= %s", typeInfo().name(), typeid(uint16_t).name());
			return 0;
		}

		inline uint8_t toByte() const {
			assert(!empty());
			if (type() == Any::Float) {
				return ((uint8_t)getFloat());
			}
			if (type() == Any::Int) {
				return ((uint8_t)getInt());
			}
			if (type() == Any::String) {
				try {
					return (boost::lexical_cast<uint8_t>(getString()));
				}
				catch (boost::bad_lexical_cast &error) {
					return 0;
				}
			}
			FIXME("Unsupported cast %s >= %s", typeInfo().name(), typeid(uint8_t).name());
			return 0;
		}
};

#endif // ANY_H
