#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

boost::any operator ! (boost::any &l);
boost::any operator + (boost::any &l);
boost::any operator - (boost::any &l);
boost::any operator % (boost::any &l, boost::any &r);
boost::any operator * (boost::any &l, boost::any &r);
boost::any operator + (boost::any &l, boost::any &r);
boost::any operator - (boost::any &l, boost::any &r);
boost::any operator / (boost::any &l, boost::any &r);
boost::any operator << (boost::any &l, boost::any &r);
boost::any operator >> (boost::any &l, boost::any &r);
boost::any operator ^ (boost::any &l, boost::any &r);
int32_t operator != (boost::any &l, boost::any &r);
int32_t operator && (boost::any &l, boost::any &r);
int32_t operator <= (boost::any &l, boost::any &r);
int32_t operator == (boost::any &l, boost::any &r);
int32_t operator >= (boost::any &l, boost::any &r);
int32_t operator || (boost::any &l, boost::any &r);
int32_t operator > (boost::any &l, boost::any &r);
int32_t operator < (boost::any &l, boost::any &r);

template<class T> T cast(const boost::any &a);

string cast_to_string(const boost::any &a);

template<class T> T cast(const boost::any &a) {
	if (typeid(T) == typeid(float)) {
		if (a.type() == typeid(float)) return boost::any_cast<float>(a);
		if (a.type() == typeid(int32_t)) {
			return ((float)boost::any_cast<int32_t>(a));
		}
		if (a.type() == typeid(uint16_t)) {
			return ((float)boost::any_cast<uint16_t>(a));
		}
		if (a.type() == typeid(uint8_t)) {
			return ((float)boost::any_cast<uint8_t>(a));
		}
		if (a.type() == typeid(string)) {
			try {
				return (boost::lexical_cast<float>(boost::any_cast<string>(a)));
			}
			catch (boost::bad_lexical_cast &error) {
				return 0.0f;
			}
		}
	}
	if (typeid(T) == typeid(int32_t)) {
		if (a.type() == typeid(int32_t)) return boost::any_cast<int32_t>(a);
		if (a.type() == typeid(float)) {
			return ((int32_t)boost::any_cast<float>(a));
		}
		if (a.type() == typeid(uint16_t)) {
			return ((int32_t)boost::any_cast<uint16_t>(a));
		}
		if (a.type() == typeid(uint8_t)) {
			return ((int32_t)boost::any_cast<uint8_t>(a));
		}
		if (a.type() == typeid(string)) {
			try {
				return (boost::lexical_cast<int32_t>(boost::any_cast<string>(a)));
			}
			catch (boost::bad_lexical_cast &error) {
				return 0;
			}
		}
	}

	if (typeid(T) == typeid(uint16_t)) {
		if (a.type() == typeid(uint16_t)) return boost::any_cast<uint16_t>(a);
		if (a.type() == typeid(float)) {
			return ((uint16_t)boost::any_cast<float>(a));
		}
		if (a.type() == typeid(int32_t)) {
			return ((uint16_t)boost::any_cast<int32_t>(a));
		}
		if (a.type() == typeid(uint8_t)) {
			return ((uint16_t)boost::any_cast<uint8_t>(a));
		}
		if (a.type() == typeid(string)) {
			try {
				return (boost::lexical_cast<uint16_t>(boost::any_cast<string>(a)));
			}
			catch (boost::bad_lexical_cast &error) {
				return 0;
			}
		}
	}

	if (typeid(T) == typeid(uint8_t)) {
		if (a.type() == typeid(uint8_t)) return boost::any_cast<uint8_t>(a);
		if (a.type() == typeid(float)) {
			return ((uint8_t)boost::any_cast<float>(a));
		}
		if (a.type() == typeid(int32_t)) {
			return ((uint8_t)boost::any_cast<int32_t>(a));
		}
		if (a.type() == typeid(uint16_t)) {
			return ((uint8_t)boost::any_cast<uint16_t>(a));
		}
		if (a.type() == typeid(string)) {
			try {
				return (boost::lexical_cast<uint8_t>(boost::any_cast<string>(a)));
			}
			catch (boost::bad_lexical_cast &error) {
				return 0;
			}
		}
	}
	FIXME("Unsupported cast %s >= %s",a.type().name(),typeid(T).name());
}

#endif


