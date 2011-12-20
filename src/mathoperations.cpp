#include "precomp.h"
#include "mathoperations.h"
#include "cbenchanted.h"
#include "any.h"


Any operator ! (Any &l) {
    if (l.type() == Any::Float) {
        return !l.getFloat();
	}
    if (l.type() == Any::Int) {
        return !l.getInt();
	}
    FIXME("Unsupported operation !%s", l.typeInfo().name());
}

Any operator + (Any &l) {
    if (l.type() == Any::Float) {
        return +l.getFloat();
	}
    if (l.type() == Any::Int) {
        return +l.getInt();
	}
    FIXME("Unsupported operation +%s", l.typeInfo().name());
}

Any operator - (Any &l) {
    if (l.type() == Any::Float) {
        return -l.getFloat();
	}
    if (l.type() == Any::Int) {
        return -l.getInt();
	}
    FIXME("Unsupported operation -%s", l.typeInfo().name());
}

Any operator % (Any &l, Any &r) {
    if (l.type() == Any::Int) {
        if (r.type() == Any::Int) {
            return l.getInt() % r.getInt();
		}
	}
    FIXME("Unsupported operation %s % %s", l.typeInfo().name(), r.typeInfo().name());
}

Any operator * (Any &l, Any &r) {
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
}

Any operator + (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.getFloat() + r.getFloat();
		}
        if (r.type() == Any::Int) {
            return l.getFloat() + r.getInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.getInt() + r.getFloat();
		}
        if (r.type() == Any::Int) {
            return l.getInt() + r.getInt();
		}
	}
    if (l.type() == Any::String) {
        /*if (r.type() == Any::Float) {
            return l.getString() + r.getFloat();
		}
        if (r.type() == Any::Int) {
            return l.getString() + r.getInt();
		}*/
        if (r.type() == Any::String) {
            return l.getString() + r.getString();
		}
	}
    FIXME("Unsupported operation %s + %s", l.typeInfo().name(), r.typeInfo().name());
}

Any operator - (Any &l, Any &r) {
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
}

Any operator / (Any &l, Any &r) {
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
}

Any operator << (Any &l, Any &r) {
    if (l.type() == Any::Int) {
        if (r.type() == Any::Int) {
            return l.getInt() << r.getInt();
		}
	}
    FIXME("Unsupported operation %s << %s", l.typeInfo().name(), r.typeInfo().name());
}

Any operator >> (Any &l, Any &r) {
    if (l.type() == Any::Int) {
        if (r.type() == Any::Int) {
            return l.getInt() >> r.getInt();
		}
	}
    FIXME("Unsupported operation %s >> %s", l.typeInfo().name(), r.typeInfo().name());
}

Any operator ^ (Any &l, Any &r) {
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
            return (int)powl(l.getInt(), r.getInt());//L: muutin pow:n powl:ksi että toimisi VC++...
		}
	}
    FIXME("Unsupported operation %s ^ %s", l.typeInfo().name(), r.typeInfo().name());
}

int32_t operator != (Any &l, Any &r) {
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
    FIXME("Unsupported operation %s != %s", l.typeInfo().name(), r.typeInfo().name());
}

int32_t operator && (Any &l, Any &r) {
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
    FIXME("Unsupported operation %s && %s", l.typeInfo().name(), r.typeInfo().name());
}

int32_t operator <= (Any &l, Any &r) {
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
}

int32_t operator == (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.getFloat() == r.getFloat();
		}
        if (r.type() == Any::Int) {
            return l.getFloat() == r.getInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.getInt() == r.getFloat();
		}
        if (r.type() == Any::Int) {
            return l.getInt() == r.getInt();
		}
	}
    if (l.type() == Any::String) {
        if (r.type() == Any::String) {
            return l.getString() == r.getString();
		}
	}
    FIXME("Unsupported operation %s == %s", l.typeInfo().name(), r.typeInfo().name());
}

int32_t operator >= (Any &l, Any &r) {
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
}

int32_t operator || (Any &l, Any &r) {
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
    FIXME("Unsupported operation %s || %s", l.typeInfo().name(), r.typeInfo().name());
}

int32_t operator > (Any &l, Any &r) {
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
}

int32_t operator < (Any &l, Any &r) {
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
}

