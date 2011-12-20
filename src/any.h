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
        Short = 3,
        Byte = 4,
        String = 5
    };
private:
    union { //Data
        float d_float;
        int32_t d_int;
        uint16_t d_short;
        uint8_t d_byte;
        string *d_string_ptr;
        void *d_ptr;
    };
    unsigned char typeId;

public:
    inline Any():typeId(Empty) {}
    inline Any(int32_t a):typeId(Int),d_int(a) { }
    inline Any(uint16_t a):typeId(Short),d_short(a) { }
    inline Any(uint8_t a):typeId(Byte),d_byte(a) { }
    inline Any(float a):typeId(Float),d_float(a) { }
    inline Any(const string &a):typeId(String),d_string_ptr(new string(a)) { }
    inline Any(const Any &a):typeId(a.typeId) {
        if (a.typeId == String)
        {
            d_string_ptr = new string(*a.d_string_ptr);
            return;
        }
        d_int = a.d_int; //Don't have to worry about data type.
    }

    inline const string &toString() const {assert(typeId == String); return *d_string_ptr; }
    inline int32_t toInt() const {assert(typeId == Int); return d_int; }
    inline uint16_t toShort() const {assert(typeId == Short); return d_short; }
    inline uint8_t toByte() const {assert(typeId == Byte); return d_byte; }
    inline float toFloat() const {assert(typeId == Float); return d_float;}
    inline bool empty() const {return typeId == Empty;}
    inline unsigned char type() const{return typeId;}
    inline const type_info &type_info() const{
        switch (typeId)
        {
        case Int:
            return typeid(int32_t);
        case Float:
            return typeid(float);
        case Short:
            return typeid(uint16_t);
        case Byte:
            return typeid(uint8_t);
        case String:
            return typeid(string);
        default:
            return typeid(void);
        }
    }

    inline string cast_to_string() const{
        try {
            if (type() == Any::String) return toString();
            if (type() == Any::Float) {
                return boost::lexical_cast<string>(toFloat());
            }
            if (type() == Any::Int) {
                return boost::lexical_cast<string>(toInt());
            }
            if (type() == Any::Short) {
                return boost::lexical_cast<string>(toShort());
            }
            if (type() == Any::Byte) {
                return boost::lexical_cast<string>(toByte());
            }
        }
        catch (boost::bad_lexical_cast &error) {
            return "";
        }
        FIXME("Unsupported cast %s >= %s",type_info().name(),typeid(string).name());
        return "";
    }

    inline int32_t cast_to_int() const {
        if (type() == Any::Int) return toInt();
        if (type() == Any::Float) {
            return ((int32_t)toFloat());
        }
        if (type() == Any::Short) {
            return ((int32_t)toShort());
        }
        if (type() == Any::Byte) {
            return ((int32_t)toByte());
        }
        if (type() == Any::String) {
            try {
                return (boost::lexical_cast<int32_t>(toString()));
            }
            catch (boost::bad_lexical_cast &error) {
                return 0;
            }
        }
        FIXME("Unsupported cast %s >= %s",type_info().name(),typeid(int32_t).name());
        return 0;
    }

    inline float cast_to_float() const {
        if (type() == Any::Float) return toFloat();
        if (type() == Any::Int) {
            return ((float)toInt());
        }
        if (type() == Any::Short) {
            return ((float)toShort());
        }
        if (type() == Any::Byte) {
            return ((float)toByte());
        }
        if (type() == Any::String) {
            try {
                return (boost::lexical_cast<float>(toString()));
            }
            catch (boost::bad_lexical_cast &error) {
                return 0.0f;
            }
        }
        FIXME("Unsupported cast %s >= %s",type_info().name(),typeid(float).name());
        return 0.0f;
    }

    inline uint16_t cast_to_short() const {
        if (type() == Any::Short) return toShort();
        if (type() == Any::Float) {
            return ((uint16_t)toFloat());
        }
        if (type() == Any::Int) {
            return ((uint16_t)toInt());
        }
        if (type() == Any::Byte) {
            return ((uint16_t)toByte());
        }
        if (type() == Any::String) {
            try {
                return (boost::lexical_cast<uint16_t>(toString()));
            }
            catch (boost::bad_lexical_cast &error) {
                return 0;
            }
        }
        FIXME("Unsupported cast %s >= %s",type_info().name(),typeid(uint16_t).name());
        return 0;
    }

    inline uint8_t cast_to_byte() const {
        if (type() == Any::Byte) return toByte();
        if (type() == Any::Float) {
            return ((uint8_t)toFloat());
        }
        if (type() == Any::Int) {
            return ((uint8_t)toInt());
        }
        if (type() == Any::Short) {
            return ((uint8_t)toShort());
        }
        if (type() == Any::String) {
            try {
                return (boost::lexical_cast<uint8_t>(toString()));
            }
            catch (boost::bad_lexical_cast &error) {
                return 0;
            }
        }
        FIXME("Unsupported cast %s >= %s",type_info().name(),typeid(uint8_t).name());
        return 0;
    }

};

#endif // ANY_H
