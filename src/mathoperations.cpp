#include "precomp.h"
#include "cbenchanted.h"

boost::any operator ! (boost::any &l) {
	if (l.type() == typeid(float)) {
		return !boost::any_cast<float>(l);
	}
	if (l.type() == typeid(int32_t)) {
		return !boost::any_cast<int32_t>(l);
	}
	FIXME("Unsupported operation !%s", l.type().name());
}

boost::any operator + (boost::any &l) {
	if (l.type() == typeid(float)) {
		return +boost::any_cast<float>(l);
	}
	if (l.type() == typeid(int32_t)) {
		return +boost::any_cast<int32_t>(l);
	}
	FIXME("Unsupported operation +%s", l.type().name());
}

boost::any operator - (boost::any &l) {
	if (l.type() == typeid(float)) {
		return -boost::any_cast<float>(l);
	}
	if (l.type() == typeid(int32_t)) {
		return -boost::any_cast<int32_t>(l);
	}
	FIXME("Unsupported operation -%s", l.type().name());
}

boost::any operator % (boost::any &l, boost::any &r) {
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) % boost::any_cast<int32_t>(r);
		}
	}
	FIXME("Unsupported operation %s % %s", l.type().name(), r.type().name());
}

boost::any operator * (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<float>(l) * boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<float>(l) * boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<int32_t>(l) * boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) * boost::any_cast<int32_t>(r);
		}
	}
	FIXME("Unsupported operation %s * %s", l.type().name(), r.type().name());
}

boost::any operator + (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<float>(l) + boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<float>(l) + boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<int32_t>(l) + boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) + boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(string)) {
		/*if (r.type() == typeid(float)) {
			return boost::any_cast<string>(l) + boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<string>(l) + boost::any_cast<int32_t>(r);
		}*/
		if (r.type() == typeid(string)) {
			return boost::any_cast<string>(l) + boost::any_cast<string>(r);
		}
	}
	FIXME("Unsupported operation %s + %s", l.type().name(), r.type().name());
}

boost::any operator - (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<float>(l) - boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<float>(l) - boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<int32_t>(l) - boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) - boost::any_cast<int32_t>(r);
		}
	}
	FIXME("Unsupported operation %s - %s", l.type().name(), r.type().name());
}

boost::any operator / (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<float>(l) / boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<float>(l) / boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<int32_t>(l) / boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) / boost::any_cast<int32_t>(r);
		}
	}
	FIXME("Unsupported operation %s / %s", l.type().name(), r.type().name());
}

boost::any operator << (boost::any &l, boost::any &r) {
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) << boost::any_cast<int32_t>(r);
		}
	}
	FIXME("Unsupported operation %s << %s", l.type().name(), r.type().name());
}

boost::any operator >> (boost::any &l, boost::any &r) {
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) >> boost::any_cast<int32_t>(r);
		}
	}
	FIXME("Unsupported operation %s >> %s", l.type().name(), r.type().name());
}

boost::any operator ^ (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return (float)pow(boost::any_cast<float>(l), boost::any_cast<float>(r));
		}
		if (r.type() == typeid(int32_t)) {
			return (float)pow(boost::any_cast<float>(l), boost::any_cast<int32_t>(r));
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return (int)pow(boost::any_cast<int32_t>(l), boost::any_cast<float>(r));
		}
		if (r.type() == typeid(int32_t)) {
			return (int)powl(boost::any_cast<int32_t>(l), boost::any_cast<int32_t>(r));//L: muutin pow:n powl:ksi että toimisi VC++...
		}
	}
	FIXME("Unsupported operation %s ^ %s", l.type().name(), r.type().name());
}

int32_t operator != (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<float>(l) != boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<float>(l) != boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<int32_t>(l) != boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) != boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(string)) {
		if (r.type() == typeid(string)) {
			return boost::any_cast<string>(l) != boost::any_cast<string>(r);
		}
	}
	FIXME("Unsupported operation %s != %s", l.type().name(), r.type().name());
}

int32_t operator && (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<float>(l) && boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<float>(l) && boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<int32_t>(l) && boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) && boost::any_cast<int32_t>(r);
		}
	}
	FIXME("Unsupported operation %s && %s", l.type().name(), r.type().name());
}

int32_t operator <= (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<float>(l) <= boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<float>(l) <= boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<int32_t>(l) <= boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) <= boost::any_cast<int32_t>(r);
		}
	}
	FIXME("Unsupported operation %s <= %s", l.type().name(), r.type().name());
}

int32_t operator == (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<float>(l) == boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<float>(l) == boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<int32_t>(l) == boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) == boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(string)) {
		if (r.type() == typeid(string)) {
			return boost::any_cast<string>(l) == boost::any_cast<string>(r);
		}
	}
	FIXME("Unsupported operation %s == %s", l.type().name(), r.type().name());
}

int32_t operator >= (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<float>(l) >= boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<float>(l) >= boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<int32_t>(l) >= boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) >= boost::any_cast<int32_t>(r);
		}
	}
	FIXME("Unsupported operation %s >= %s", l.type().name(), r.type().name());
}

int32_t operator || (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<float>(l) || boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<float>(l) || boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<int32_t>(l) || boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) || boost::any_cast<int32_t>(r);
		}
	}
	FIXME("Unsupported operation %s || %s", l.type().name(), r.type().name());
}

int32_t operator > (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<float>(l) > boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<float>(l) > boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<int32_t>(l) > boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) > boost::any_cast<int32_t>(r);
		}
	}
	FIXME("Unsupported operation %s >= %s", l.type().name(), r.type().name());
}

int32_t operator < (boost::any &l, boost::any &r) {
	if (l.type() == typeid(float)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<float>(l) < boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<float>(l) < boost::any_cast<int32_t>(r);
		}
	}
	if (l.type() == typeid(int32_t)) {
		if (r.type() == typeid(float)) {
			return boost::any_cast<int32_t>(l) < boost::any_cast<float>(r);
		}
		if (r.type() == typeid(int32_t)) {
			return boost::any_cast<int32_t>(l) < boost::any_cast<int32_t>(r);
		}
	}
	FIXME("Unsupported operation %s >= %s", l.type().name(), r.type().name());
}

