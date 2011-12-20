#include "precomp.h"
#include "mathoperations.h"
#include "cbenchanted.h"
#include "any.h"


Any operator ! (Any &l) {
    if (l.type() == Any::Float) {
        return !l.toFloat();
	}
    if (l.type() == Any::Int) {
        return !l.toInt();
	}
    FIXME("Unsupported operation !%s", l.type_info().name());
}

Any operator + (Any &l) {
    if (l.type() == Any::Float) {
        return +l.toFloat();
	}
    if (l.type() == Any::Int) {
        return +l.toInt();
	}
    FIXME("Unsupported operation +%s", l.type_info().name());
}

Any operator - (Any &l) {
    if (l.type() == Any::Float) {
        return -l.toFloat();
	}
    if (l.type() == Any::Int) {
        return -l.toInt();
	}
    FIXME("Unsupported operation -%s", l.type_info().name());
}

Any operator % (Any &l, Any &r) {
    if (l.type() == Any::Int) {
        if (r.type() == Any::Int) {
            return l.toInt() % r.toInt();
		}
	}
    FIXME("Unsupported operation %s % %s", l.type_info().name(), r.type_info().name());
}

Any operator * (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.toFloat() * r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toFloat() * r.toInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.toInt() * r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toInt() * r.toInt();
		}
	}
    FIXME("Unsupported operation %s * %s", l.type_info().name(), r.type_info().name());
}

Any operator + (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.toFloat() + r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toFloat() + r.toInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.toInt() + r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toInt() + r.toInt();
		}
	}
    if (l.type() == Any::String) {
        /*if (r.type() == Any::Float) {
            return l.toString() + r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toString() + r.toInt();
		}*/
        if (r.type() == Any::String) {
            return l.toString() + r.toString();
		}
	}
    FIXME("Unsupported operation %s + %s", l.type_info().name(), r.type_info().name());
}

Any operator - (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.toFloat() - r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toFloat() - r.toInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.toInt() - r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toInt() - r.toInt();
		}
	}
    FIXME("Unsupported operation %s - %s", l.type_info().name(), r.type_info().name());
}

Any operator / (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.toFloat() / r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toFloat() / r.toInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.toInt() / r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toInt() / r.toInt();
		}
	}
    FIXME("Unsupported operation %s / %s", l.type_info().name(), r.type_info().name());
}

Any operator << (Any &l, Any &r) {
    if (l.type() == Any::Int) {
        if (r.type() == Any::Int) {
            return l.toInt() << r.toInt();
		}
	}
    FIXME("Unsupported operation %s << %s", l.type_info().name(), r.type_info().name());
}

Any operator >> (Any &l, Any &r) {
    if (l.type() == Any::Int) {
        if (r.type() == Any::Int) {
            return l.toInt() >> r.toInt();
		}
	}
    FIXME("Unsupported operation %s >> %s", l.type_info().name(), r.type_info().name());
}

Any operator ^ (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return (float)pow(l.toFloat(), r.toFloat());
		}
        if (r.type() == Any::Int) {
            return (float)pow(l.toFloat(), r.toInt());
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return (int)pow(l.toInt(), r.toFloat());
		}
        if (r.type() == Any::Int) {
            return (int)powl(l.toInt(), r.toInt());//L: muutin pow:n powl:ksi että toimisi VC++...
		}
	}
    FIXME("Unsupported operation %s ^ %s", l.type_info().name(), r.type_info().name());
}

int32_t operator != (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.toFloat() != r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toFloat() != r.toInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.toInt() != r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toInt() != r.toInt();
		}
	}
    if (l.type() == Any::String) {
        if (r.type() == Any::String) {
            return l.toString() != r.toString();
		}
	}
    FIXME("Unsupported operation %s != %s", l.type_info().name(), r.type_info().name());
}

int32_t operator && (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.toFloat() && r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toFloat() && r.toInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.toInt() && r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toInt() && r.toInt();
		}
	}
    FIXME("Unsupported operation %s && %s", l.type_info().name(), r.type_info().name());
}

int32_t operator <= (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.toFloat() <= r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toFloat() <= r.toInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.toInt() <= r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toInt() <= r.toInt();
		}
	}
    FIXME("Unsupported operation %s <= %s", l.type_info().name(), r.type_info().name());
}

int32_t operator == (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.toFloat() == r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toFloat() == r.toInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.toInt() == r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toInt() == r.toInt();
		}
	}
    if (l.type() == Any::String) {
        if (r.type() == Any::String) {
            return l.toString() == r.toString();
		}
	}
    FIXME("Unsupported operation %s == %s", l.type_info().name(), r.type_info().name());
}

int32_t operator >= (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.toFloat() >= r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toFloat() >= r.toInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.toInt() >= r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toInt() >= r.toInt();
		}
	}
    FIXME("Unsupported operation %s >= %s", l.type_info().name(), r.type_info().name());
}

int32_t operator || (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.toFloat() || r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toFloat() || r.toInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.toInt() || r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toInt() || r.toInt();
		}
	}
    FIXME("Unsupported operation %s || %s", l.type_info().name(), r.type_info().name());
}

int32_t operator > (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.toFloat() > r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toFloat() > r.toInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.toInt() > r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toInt() > r.toInt();
		}
	}
    FIXME("Unsupported operation %s >= %s", l.type_info().name(), r.type_info().name());
}

int32_t operator < (Any &l, Any &r) {
    if (l.type() == Any::Float) {
        if (r.type() == Any::Float) {
            return l.toFloat() < r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toFloat() < r.toInt();
		}
	}
    if (l.type() == Any::Int) {
        if (r.type() == Any::Float) {
            return l.toInt() < r.toFloat();
		}
        if (r.type() == Any::Int) {
            return l.toInt() < r.toInt();
		}
	}
    FIXME("Unsupported operation %s >= %s", l.type_info().name(), r.type_info().name());
}

