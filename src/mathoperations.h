#ifndef MATHOPERATIONS_H
#define MATHOPERATIONS_H
#include "any.h"
#include "variablestack.h"
#include <cmath>
#include "isstring.h"
#include "cbenchanted.h"
#include "errorsystem.h"

FORCEINLINE int32_t cbpow(int32_t a, int32_t b) {
	double ret = pow(double(a), double(b));
	if (ret > 0x7FFFFFFF) {
		return 0xFFFFFFFF;
	}
	return ret;
}


FORCEINLINE void Any::addition(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (b.typeId) {
		case Int: {
			switch (a.typeId) {
				case Int: a.dInt = a.dInt + b.dInt; return;
				case Float: a.dFloat = a.dFloat + b.dInt; return;
				case String: {
					ISString s = a.getString();
					ISString res(s.getRef() + boost::lexical_cast<string>(b.dInt));
					res.requireEncoding(s.isEncodingRequired());
					a = res;
					return;
				}
			}
			break;
		}
		case Float: {
			switch (a.typeId) {
				case Int:
					a.dFloat = a.dInt + b.dFloat;
					a.typeId = Float;
					return;
				case Float: a.dFloat = a.dFloat + b.dFloat; return;
				case String: {
					ISString s = a.getString();
					ISString res(s.getRef() + boost::lexical_cast<string>(b.dFloat));
					res.requireEncoding(s.isEncodingRequired());
					a = res;
					return;
				}
			}
			break;
		}
		case String: {
			switch (a.typeId) {
				case Int: {
					ISString s = b.getString();
					ISString res(boost::lexical_cast<string>(a.dInt) + s.getRef());
					res.requireEncoding(s.isEncodingRequired());
					a = res;
					return;
				}
				case Float: {
					ISString s = b.getString();
					ISString res(boost::lexical_cast<string>(a.dFloat) + s.getRef());
					res.requireEncoding(s.isEncodingRequired());
					a = res;
					return;
				}
				case String: {
					ISString as = a.getString();
					ISString bs = b.getString();
					ISString res(as + bs);
					res.requireEncoding(as.isEncodingRequired() || bs.isEncodingRequired());
					a = res;
					return;
				}
			}
			break;
		}	
	}
	FIXME("Unsupported operation %s + %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::multiplication(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (b.typeId) {
		case Int: {
			switch (a.typeId) {
				case Int:
					a.dInt = a.dInt * b.dInt;
					return;
				case Float:
					a.dFloat = a.dFloat * b.dInt;
					return;
			}
			break;
		}
		case Float: {
			switch (a.typeId) {
				case Int:
					a.dFloat = a.dInt * b.dFloat;
					a.typeId = Float;
					return;
				case Float: a.dFloat = a.dFloat * b.dFloat; return;
			}
			break;
		}
	}
	FIXME("Unsupported operation %s * %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::subtraction(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (b.typeId) {
		case Int:
			switch (a.typeId) {
				case Int: a.dInt = a.dInt - b.dInt; return;
				case Float: a.dFloat = a.dFloat - b.dInt; return;
			}
			break;
		case Float:
			switch (a.typeId) {
				case Int:
					a.dFloat = a.dInt - b.dFloat;
					a.typeId = Float;
					return;
				case Float: a.dFloat = a.dFloat - b.dFloat; return;
			}
			break;
	}

	FIXME("Unsupported operation %s - %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::division(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (b.typeId) {
		case Int:
			switch (a.typeId) {
				case Int:
					if (b.dInt == 0) {
						CBEnchanted::instance()->errors->createError("Integer divided by zero", "", "Integer pwned by Zero");
						a.dInt = 0;
						return;
					}
					a.dInt = a.dInt / b.dInt;
					return;
				case Float: a.dFloat = a.dFloat / (float)b.dInt; return;
			}
			break;
		case Float:
			switch (a.typeId) {
				case Int:
					a.dFloat = (float)a.dInt / b.dFloat;
					a.typeId = Float;
					return;
				case Float: a.dFloat = a.dFloat / b.dFloat; return;
			}
			break;
	}
	FIXME("Unsupported operation %s / %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::modulo(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (b.typeId) {
		case Int:
			switch (a.typeId) {
				case Int:
					if (b.dInt == 0) {
						CBEnchanted::instance()->errors->createError("Integer divided by zero", "This happened when using the Mod operator", "Integer pwned by Zero");
						a.dInt = 0;
						return;
					}
					a.dInt = a.dInt % b.dInt;
					return;
				case Float: a.dFloat = fmod((double)a.dFloat, (double)b.dInt); return;
			}
			break;	
		case Float:
			switch (a.typeId) {
				case Int:
					a.dFloat = fmod((double)a.dInt, (double)b.dFloat);
					a.typeId = Float;
					return;
				case Float: a.dFloat = fmod(a.dFloat, b.dFloat); return;
			}
			break;	
	}
	FIXME("Unsupported operation %s %% %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::shl(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	a.dInt = a.toInt() << b.toInt();
	a.typeId = Int;
}

FORCEINLINE void Any::sar(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	a.dInt = a.toInt() >> b.toInt();
	a.typeId = Int;
}

FORCEINLINE void Any::shr(VariableStack *s) {
	union LogicalShiftHelper {
			int32_t i;
			uint32_t u;
	};
	LogicalShiftHelper ah;
	LogicalShiftHelper bh;
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	ah.i = a.toInt();
	bh.i = b.toInt();
	*reinterpret_cast<uint32_t*>(&a.dInt) = ah.u >> bh.u;
	a.typeId = Int;
}

FORCEINLINE void Any::power(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (b.typeId) {
		case Int:
			switch (a.typeId) {
				case Int: a.dInt = cbpow(a.dInt, b.dInt); return;
				case Float: a.dFloat = pow((double)a.dFloat, (double)b.dInt); return;
			}
			break;
		case Float:
			switch (a.typeId) {
				case Int:
					a.dFloat = pow((double)a.dInt, (double)b.dFloat);
					a.typeId = Float;
					return;
				case Float: a.dFloat = powf(a.dFloat, b.dFloat); return;
			}
			break;
	}
}

FORCEINLINE void Any::equal(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (a.typeId) {
		case Int:
			switch (b.typeId) {
				case Int:
					a.dInt = a.dInt == b.dInt;
					return;	
				case Float:
					a.dInt = a.dInt == b.dFloat;
					return;
				case String:
					a.dInt = a.toString() == b.getString();
					return;
			}
		case Float:
			switch (b.typeId) {
				case Float:
					a.typeId = Int;
					a.dInt = a.dFloat == b.dFloat;
					return;
				case Int:
					a.typeId = Int;
					a.dInt = a.dFloat == b.dInt;
					return;
				case String:
					a.typeId = Int;
					a.dInt = a.toString() == b.getString();
					return;
			}
		case String:
			switch (b.typeId) {
				case Int:
				case Float:
					a.typeId = Int;
					a.dInt = a.getString() == b.toString();
					return;
				case String:
					a.typeId = Int;
					a.dInt = a.getString() == b.getString();
					return;
			}
			break;
		case TypePtr:
			if (b.typeId == TypePtr) {
				a.typeId = Int;
				a.dInt = a.dPtr == b.dPtr;
				return;
			}
			break;
	}
	FIXME("Unsupported operation %s == %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::notEqual(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (a.typeId) {
		case Int:
			switch (b.typeId) {
				case Int: a.dInt = a.dInt != b.dInt; return;
				case Float: a.dInt = a.dInt != b.dFloat; return;
				case String: a.dInt = a.toString() != b.getString(); return;
			}
			break;
		case Float:
			switch (b.typeId) {
				case Int: a.typeId = Int; a.dInt = a.dFloat != b.dInt; return;
				case Float: a.typeId = Int; a.dInt = a.dFloat != b.dFloat; return;
				case String: a.typeId = Int; a.dInt = a.toString() != b.getString(); return;
			}
			break;
		case String:
			switch (b.typeId) {
				case Int:
					a.typeId = Int;
					a.dInt = a.getString() != b.toString();
					return;
				case Float:
					a.typeId = Int;
					a.dInt = a.getString() != b.toString();
					return;
				case String:
					a.typeId = Int;
					a.dInt = a.getString() != b.getString();
					return;
			}
			break;
		case TypePtr:
			if (b.typeId == TypePtr) {
				a.typeId = Int;
				a.dInt = a.dPtr != b.dPtr;
				return;
			}
			break;
	}

	FIXME("Unsupported operation %s != %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::AND(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	a.dInt = a.toBool() && b.toBool();
	a.typeId = Int;
}

FORCEINLINE void Any::OR(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	a.dInt = a.toBool() || b.toBool();
	a.typeId = Int;
}

FORCEINLINE void Any::XOR(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	a.dInt = a.toBool() ^ b.toBool();
	a.typeId = Int;
}

FORCEINLINE void Any::NOT(VariableStack *s) {
	Any &a = s->stackArray[s->stackLevel - 1];
	a.dInt = !a.toBool();
	a.typeId = Int;
}

FORCEINLINE void Any::greaterThan(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (a.typeId) {
		case Int:
			switch (b.typeId) {
				case Int: a.dInt = a.dInt > b.dInt; return;
				case Float: a.dInt = a.dInt > b.dFloat; return;
				case String: a.dInt = a.toString() > b.getString(); return;
			}
			break;
		case Float:
			switch (b.typeId) {
				case Int: a.typeId = Int; a.dInt = a.dFloat > b.dInt; return;
				case Float: a.typeId = Int; a.dInt = a.dFloat > b.dFloat; return;
				case String: a.typeId = Int; a.dInt = a.toString() > b.getString(); return;
			}
			break;
		case String:
			switch (b.typeId) {
				case Int: a.typeId = Int; a.dInt = a.getString() > b.toString(); return;
				case Float: a.typeId = Int; a.dInt = a.getString() > b.toString(); return;
				case String: a.typeId = Int; a.dInt = a.getString() > b.getString(); return;
			}
			break;
	}
	FIXME("Unsupported operation %s > %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::greaterThanOrEqual(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (a.typeId) {
		case Int:
			switch (b.typeId) {
				case Int: a.dInt = a.dInt >= b.dInt; return;
				case Float: a.dInt = a.dInt >= b.dFloat; return;
				case String: a.dInt = a.toString() >= b.getString(); return;
			}
			break;
		case Float:
			switch (b.typeId) {
				case Int: a.typeId = Int; a.dInt = a.dFloat >= b.dInt; return;
				case Float: a.typeId = Int; a.dInt = a.dFloat >= b.dFloat; return;
				case String: a.typeId = Int; a.dInt = a.toString() >= b.getString(); return;
			}
			break;
		case String:
			switch (b.typeId) {
				case Int: a.typeId = Int; a.dInt = a.getString() >= b.toString(); return;
				case Float: a.typeId = Int; a.dInt = a.getString() >= b.toString(); return;
				case String: a.typeId = Int; a.dInt = a.getString() >= b.getString(); return;
			}
			break;
	}
	FIXME("Unsupported operation %s >= %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::lessThan(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (a.typeId) {
		case Int:
			switch (b.typeId) {
				case Int: a.dInt = a.dInt < b.dInt; return;
				case Float: a.dInt = a.dInt < b.dFloat; return;
				case String: a.dInt = a.toString() < b.getString(); return;
			}
			break;
		case Float:
			switch (b.typeId) {
				case Int: a.typeId = Int; a.dInt = a.dFloat < b.dInt; return;
				case Float: a.typeId = Int; a.dInt = a.dFloat < b.dFloat; return;
				case String: a.typeId = Int; a.dInt = a.toString() < b.getString(); return;
			}
			break;
		case String:
			switch (b.typeId) {
				case Int: a.typeId = Int; a.dInt = a.getString() < b.toString(); return;
				case Float: a.typeId = Int; a.dInt = a.getString() < b.toString(); return;
				case String: a.typeId = Int; a.dInt = a.getString() < b.getString(); return;
			}
			break;
	}
	FIXME("Unsupported operation %s < %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::lessThanOrEqual(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (a.typeId) {
		case Int:
			switch (b.typeId) {
				case Int: a.dInt = a.dInt <= b.dInt; return;
				case Float: a.dInt = a.dInt <= b.dFloat; return;
				case String: a.dInt = a.toString() <= b.getString(); return;
			}
			break;
		case Float:
			switch (b.typeId) {
				case Int:
					a.typeId = Int;
					a.dInt = a.dFloat <= b.dInt;
					return;
				case Float:
					a.typeId = Int;
					a.dInt = a.dFloat <= b.dFloat;
					return;
				case String:
					a.typeId = Int;
					a.dInt = a.toString() <= b.getString();
					return;
			}
			break;
		case String:
			switch (b.typeId) {
				case Int:
					a.typeId = Int;
					a.dInt = a.getString() <= b.toString();
					return;
				case Float:
					a.typeId = Int;
					a.dInt = a.getString() <= b.toString();
					return;
				case String:
					a.typeId = Int;
					a.dInt = a.getString() <= b.getString();
					return;
			}
			break;
	}
	FIXME("Unsupported operation %s <= %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::unaryMinus(VariableStack *s) {
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (a.typeId) {
		case Int: a.dInt = -a.dInt; return;
		case Float: a.dFloat = -a.dFloat; return;
	}
	FIXME("Unsupported operation -%s", a.typeInfo().name());
}

FORCEINLINE void Any::unaryPlus(VariableStack *s) {
	Any &a = s->stackArray[s->stackLevel - 1];
	switch (a.typeId) {
		case Int: a.dInt = abs(a.dInt); return;
		case Float: a.dFloat = abs(a.dFloat); return;
	}
	FIXME("Unsupported operation +%s", a.typeInfo().name());
}

#endif // MATHOPERATIONS_H
