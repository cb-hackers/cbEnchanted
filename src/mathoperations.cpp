#include "precomp.h"
#include "mathoperations.h"
#include "cbenchanted.h"
#include "any.h"


Any operator ! (const Any &l) {
	if (l.type() == Any::Float) {
		return !l.getFloat();
	}
	if (l.type() == Any::Int) {
		return !l.getInt();
	}
	if (l.type() == Any::TypePtr) {
		return !l.getTypePtr();
	}
	FIXME("Unsupported operation !%s", l.typeInfo().name());
	return 0;
}

Any operator + (const Any &l) {
	if (l.type() == Any::Float) {
		return +l.getFloat();
	}
	if (l.type() == Any::Int) {
		return +l.getInt();
	}
	FIXME("Unsupported operation +%s", l.typeInfo().name());
	return 0;
}

Any operator - (const Any &l) {
	if (l.type() == Any::Float) {
		return -l.getFloat();
	}
	if (l.type() == Any::Int) {
		return -l.getInt();
	}
	FIXME("Unsupported operation -%s", l.typeInfo().name());
	return 0;
}

Any operator % (const Any &l, const Any &r) {
	return l.toInt() % r.toInt();
	FIXME("Unsupported operation %s % %s", l.typeInfo().name(), r.typeInfo().name());
}

Any operator * (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return l.getFloat() * r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getFloat() * r.getInt();
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return l.getInt() * r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getInt() * r.getInt();
		}
	}
	FIXME("Unsupported operation %s * %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any operator + (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return l.getFloat() + r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getFloat() + r.getInt();
		}
		if (r.type() == Any::String) {
			return boost::lexical_cast<string>(l.getFloat()) + r.getString();
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return l.getInt() + r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getInt() + r.getInt();
		}
		if (r.type() == Any::String) {
			return boost::lexical_cast<string>(l.getInt()) + r.getString();
		}
	}
	if (l.type() == Any::String) {
		if (r.type() == Any::Float) {
			return l.getString() + boost::lexical_cast<string>(r.getFloat());
		}
		if (r.type() == Any::Int) {
			return l.getString() + boost::lexical_cast<string>(r.getInt());
		}
		if (r.type() == Any::String) {
			return l.getString() + r.getString();
		}
	}
	FIXME("Unsupported operation %s + %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any operator - (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return l.getFloat() - r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getFloat() - r.getInt();
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return l.getInt() - r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getInt() - r.getInt();
		}
	}
	FIXME("Unsupported operation %s - %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any operator / (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return l.getFloat() / r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getFloat() / r.getInt();
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return l.getInt() / r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getInt() / r.getInt();
		}
	}
	FIXME("Unsupported operation %s / %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any operator << (const Any &l, const Any &r) {
	if (l.type() == Any::Int) {
		if (r.type() == Any::Int) {
			int32_t a = l.getInt();
			int32_t b = r.getInt();
			uint32_t ret = *reinterpret_cast<uint32_t*>( &a ) << *reinterpret_cast<uint32_t*>( &b );
			return *reinterpret_cast<int32_t*>( &ret );
		}
	}
	FIXME("Unsupported operation %s << %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any sar (const Any &l, const Any &r) {
	if (l.type() == Any::Int) {
		if (r.type() == Any::Int) {
			return l.getInt() >> l.getInt();
		}
	}
	FIXME("Unsupported operation %s << %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

Any shr (const Any &l, const Any &r) {
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

Any operator ^ (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return (float)pow(l.getFloat(), r.getFloat());
		}
		if (r.type() == Any::Int) {
			return (float)pow(l.getFloat(), r.getInt());
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return (int)pow(l.getInt(), r.getFloat());
		}
		if (r.type() == Any::Int) {
			return (int)powl(l.getInt(), r.getInt());
		}
	}
	FIXME("Unsupported operation %s ^ %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t operator != (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return l.getFloat() != r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getFloat() != r.getInt();
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return l.getInt() != r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getInt() != r.getInt();
		}
	}
	if (l.type() == Any::String) {
		if (r.type() == Any::String) {
			return l.getString() != r.getString();
		}
	}
	if (l.type() == Any::TypePtr) {
		if (r.type() == Any::TypePtr) {
			return r.getTypePtr() != l.getTypePtr();
		}
	}
	FIXME("Unsupported operation %s != %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t operator && (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return l.getFloat() && r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getFloat() && r.getInt();
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return l.getInt() && r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getInt() && r.getInt();
		}
	}
	if (l.type() == Any::TypePtr) {
		if (r.type() == Any::TypePtr) {
			return r.getTypePtr() && l.getTypePtr();
		}
	}
	FIXME("Unsupported operation %s && %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t operator <= (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return l.getFloat() <= r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getFloat() <= r.getInt();
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return l.getInt() <= r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getInt() <= r.getInt();
		}
	}
	FIXME("Unsupported operation %s <= %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t operator == (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return l.getFloat() == r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getFloat() == r.getInt();
		}
		if (r.type() == Any::String) {
			try {
				return l.getFloat() == boost::lexical_cast<float>(r.getString().getRef());
			}
			catch( boost::bad_lexical_cast &error) {
				return 0;
			}
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return l.getInt() == r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getInt() == r.getInt();
		}
		if (r.type() == Any::String) {
			try {
				return l.getInt() == boost::lexical_cast<int32_t>(r.getString().getRef());
			}
			catch( boost::bad_lexical_cast &error) {
				return 0;
			}
		}
	}
	if (l.type() == Any::String) {
		if (r.type() == Any::String) {
			return l.getString() == r.getString();
		}
		if (r.type() == Any::Int) {
			try {
				return boost::lexical_cast<int32_t>(l.getString().getRef()) == r.getInt();
			}
			catch( boost::bad_lexical_cast &error) {
				return 0;
			}
		}
		if (r.type() == Any::Float) {
			try {
				return boost::lexical_cast<float>(l.getString().getRef()) == r.getFloat();
			}
			catch( boost::bad_lexical_cast &error) {
				return 0;
			}
		}
	}
	if (l.type() == Any::TypePtr) {
		if (r.type() == Any::TypePtr) {
			return r.getTypePtr() == l.getTypePtr();
		}
	}
	FIXME("Unsupported operation %s == %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t operator >= (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return l.getFloat() >= r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getFloat() >= r.getInt();
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return l.getInt() >= r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getInt() >= r.getInt();
		}
	}
	FIXME("Unsupported operation %s >= %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t operator || (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return l.getFloat() || r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getFloat() || r.getInt();
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return l.getInt() || r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getInt() || r.getInt();
		}
	}
	if (l.type() == Any::TypePtr) {
		if (r.type() == Any::TypePtr) {
			return r.getTypePtr() || l.getTypePtr();
		}
	}
	FIXME("Unsupported operation %s || %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t operator > (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return l.getFloat() > r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getFloat() > r.getInt();
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return l.getInt() > r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getInt() > r.getInt();
		}
	}
	FIXME("Unsupported operation %s >= %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

int32_t operator < (const Any &l, const Any &r) {
	if (l.type() == Any::Float) {
		if (r.type() == Any::Float) {
			return l.getFloat() < r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getFloat() < r.getInt();
		}
	}
	if (l.type() == Any::Int) {
		if (r.type() == Any::Float) {
			return l.getInt() < r.getFloat();
		}
		if (r.type() == Any::Int) {
			return l.getInt() < r.getInt();
		}
	}
	FIXME("Unsupported operation %s >= %s", l.typeInfo().name(), r.typeInfo().name());
	return 0;
}

