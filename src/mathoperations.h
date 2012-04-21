#ifndef MATHOPERATIONS_H
#define MATHOPERATIONS_H
#include "any.h"
#include "variablestack.h"
#include <cmath>

FORCEINLINE int32_t cbpow(int32_t a, int32_t b) {
	double ret = pow(double(a), double(b));
	if (ret > 0x7FFFFFFF) {
		return 0xFFFFFFFF;
	}
	return ret;
}


FORCEINLINE void Any::addition(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	if (b.typeId == Int) {
		if (a.typeId == Int) {
			a.dInt = a.dInt + b.dInt;
			return;
		}
		if (a.typeId == Float) {
			a.dFloat = a.dFloat + b.dInt;
			return;
		}
		if (a.typeId == String) {
			a = a.getString().getRef() + boost::lexical_cast<string>(b.dInt);
			return;
		}
	}
	if (b.typeId == Float) {
		if (a.typeId == Float) {
			a.dFloat = a.dFloat + b.dFloat;
			return;
		}
		if (a.typeId == Int) {
			a.dFloat = a.dInt + b.dFloat;
			a.typeId = Float;
			return;
		}
		if (a.typeId == String) {
			a = a.getString().getRef() + boost::lexical_cast<string>(b.dFloat);
			return;
		}
	}
	if (b.typeId == String) {
		if (a.typeId == Float) {
			a = boost::lexical_cast<string>(a.dFloat) + b.getString().getRef();
			return;
		}
		if (a.typeId == Int) {
			a = boost::lexical_cast<string>(a.dInt) + b.getString().getRef();
			return;
		}
		if (a.typeId == String) {
			a = a.getString().getRef() + b.getString().getRef();
			return;
		}
	}
	FIXME("Unsupported operation %s + %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::multiplication(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	if (b.typeId == Float) {
		if (a.typeId == Float) {
			a.dFloat = a.dFloat * b.dFloat;
			return;
		}
		if (a.typeId == Int) {
			a.dFloat = a.dInt * b.dFloat;
			a.typeId = Float;
			return;
		}
	}
	if (b.typeId == Int) {
		if (a.typeId == Float) {
			a.dFloat = a.dFloat * b.dInt;
			return;
		}
		if (a.typeId == Int) {
			a.dInt = a.dInt * b.dInt;
			return;
		}
	}
	FIXME("Unsupported operation %s * %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::subtraction(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	if (b.typeId == Float) {
		if (a.typeId == Float) {
			a.dFloat = a.dFloat - b.dFloat;
			return;
		}
		if (a.typeId == Int) {
			a.dFloat = a.dInt - b.dFloat;
			a.typeId = Float;
			return;
		}
	}
	if (b.typeId == Int) {
		if (a.typeId == Float) {
			a.dFloat = a.dFloat - b.dInt;
			return;
		}
		if (a.typeId == Int) {
			a.dInt = a.dInt - b.dInt;
			return;
		}
	}
	FIXME("Unsupported operation %s * %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::division(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	if (b.typeId == Float) {
		if (a.typeId == Float) {
			a.dFloat = a.dFloat / b.dFloat;
			return;
		}
		if (a.typeId == Int) {
			a.dFloat = (float)a.dInt / b.dFloat;
			a.typeId = Float;
			return;
		}
	}
	if (b.typeId == Int) {
		if (a.typeId == Float) {
			a.dFloat = a.dFloat / (float)b.dInt;
			return;
		}
		if (a.typeId == Int) {
			a.dInt = a.dInt / b.dInt;
			return;
		}
	}
	FIXME("Unsupported operation %s * %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::modulo(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	if (b.typeId == Float) {
		if (a.typeId == Float) {
			a.dFloat = fmod(a.dFloat, b.dFloat);
			return;
		}
		if (a.typeId == Int) {
			a.dFloat = fmod((double)a.dInt, (double)b.dFloat);
			a.typeId = Float;
			return;
		}
	}
	if (b.typeId == Int) {
		if (a.typeId == Float) {
			a.dFloat = fmod((double)a.dFloat, (double)b.dInt);
			return;
		}
		if (a.typeId == Int) {
			a.dInt = a.dInt % b.dInt;
			return;
		}
	}
	FIXME("Unsupported operation %s %% %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::shl(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	a.dInt = a.toInt() << b.toInt();
	a.typeId = Int;
}

FORCEINLINE void Any::sar(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
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
	Any &a = s->stackArray[s->stackLevel-1];
	ah.i = a.toInt();
	bh.i = b.toInt();
	*reinterpret_cast<uint32_t*>(&a.dInt) = ah.u >> bh.u;
	a.typeId = Int;
}

FORCEINLINE void Any::power(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	if (b.typeId == Float) {
		if (a.typeId == Float) {
			a.dFloat = powf(a.dFloat, b.dFloat);
			return;
		}
		if (a.typeId == Int) {
			a.dFloat = pow((double)a.dInt, (double)b.dFloat);
			a.typeId = Float;
			return;
		}
	}
	if (b.typeId == Int) {
		if (a.typeId == Int) {
			a.dInt = cbpow(a.dInt, b.dInt);
			return;
		}
		if (a.typeId == Float) {
			a.dFloat = pow((double)a.dFloat, (double)b.dInt);
			return;
		}
	}
}

FORCEINLINE void Any::equal(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	if (a.typeId == Int) {
		if (b.typeId == Int) {
			a.dInt = a.dInt == b.dInt;
			return;
		}
		if (b.typeId == Float) {
			a.dInt = a.dInt == b.dFloat;
			return;
		}
		if (b.typeId == String) {
			a.dInt = a.toString() == b.getString();
			return;
		}
	}
	if (a.typeId == Float) {
		a.typeId = Int;
		if (b.typeId == Float) {
			a.dInt = a.dFloat == b.dFloat;
			return;
		}
		if (b.typeId == Int) {
			a.dInt = a.dFloat == b.dInt;
			return;
		}
		if (b.typeId == String) {
			a.dInt = a.toString() == b.getString();
			return;
		}
	}
	if (a.typeId == TypePtr && b.typeId == TypePtr) {
		a.typeId = Int;
		a.dInt = a.dPtr == b.dPtr;
		return;
	}
	if (a.typeId == String) {
		a.typeId = Int;
		if (b.typeId == String) {
			a.dInt = a.getString() == b.getString();
			return;
		}
		if (b.typeId == Int) {
			a.dInt = a.getString() == b.toString();
			return;
		}
		if (b.typeId == Float) {
			a.dInt = a.getString() == b.toString();
			return;
		}
	}
	FIXME("Unsupported operation %s == %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::notEqual(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	if (a.typeId == Int) {
		if (b.typeId == Int) {
			a.dInt = a.dInt != b.dInt;
			return;
		}
		if (b.typeId == Float) {
			a.dInt = a.dInt != b.dFloat;
			return;
		}
		if (b.typeId == String) {
			a.dInt = a.toString() != b.getString();
			return;
		}
	}
	if (a.typeId == Float) {
		a.typeId = Int;
		if (b.typeId == Float) {
			a.dInt = a.dFloat != b.dFloat;
			return;
		}
		if (b.typeId == Int) {
			a.dInt = a.dFloat != b.dInt;
			return;
		}
		if (b.typeId == String) {
			a.dInt = a.toString() != b.getString();
			return;
		}
	}
	if (a.typeId == TypePtr && b.typeId == TypePtr) {
		a.typeId = Int;
		a.dInt = a.dPtr != b.dPtr;
		return;
	}
	if (a.typeId == String) {
		a.typeId = Int;
		if (b.typeId == String) {
			a.dInt = a.getString() != b.getString();
			return;
		}
		if (b.typeId == Int) {
			a.dInt = a.getString() != b.toString();
			return;
		}
		if (b.typeId == Float) {
			a.dInt = a.getString() != b.toString();
			return;
		}
	}
	FIXME("Unsupported operation %s != %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::AND(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	a.dInt = a.toBool() && b.toBool();
	a.typeId = Int;
}

FORCEINLINE void Any::OR(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	a.dInt = a.toBool() && b.toBool();
	a.typeId = Int;
}

FORCEINLINE void Any::XOR(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	a.dInt = a.toBool() ^ b.toBool();
	a.typeId = Int;
}

FORCEINLINE void Any::NOT(VariableStack *s) {
	Any &a = s->stackArray[s->stackLevel-1];
	a.dInt = !a.toBool();
	a.typeId = Int;
}



FORCEINLINE void Any::greaterThan(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	if (a.typeId == Int) {
		if (b.typeId == Int) {
			a.dInt = a.dInt > b.dInt;
			return;
		}
		if (b.typeId == Float) {
			a.dInt = a.dInt > b.dFloat;
			return;
		}
		if (b.typeId == String) {
			a.dInt = a.toString() > b.getString();
			return;
		}
	}
	if (a.typeId == Float) {
		a.typeId = Int;
		if (b.typeId == Float) {
			a.dInt = a.dFloat > b.dFloat;
			return;
		}
		if (b.typeId == Int) {
			a.dInt = a.dFloat > b.dInt;
			return;
		}
		if (b.typeId == String) {
			a.dInt = a.toString() > b.getString();
			return;
		}
	}
	if (a.typeId == String) {
		a.typeId = Int;
		if (b.typeId == String) {
			a.dInt = a.getString() > b.getString();
			return;
		}
		if (b.typeId == Int) {
			a.dInt = a.getString() > b.toString();
			return;
		}
		if (b.typeId == Float) {
			a.dInt = a.getString() > b.toString();
			return;
		}
	}
	FIXME("Unsupported operation %s > %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::greaterThanOrEqual(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	if (a.typeId == Int) {
		if (b.typeId == Int) {
			a.dInt = a.dInt >= b.dInt;
			return;
		}
		if (b.typeId == Float) {
			a.dInt = a.dInt >= b.dFloat;
			return;
		}
		if (b.typeId == String) {
			a.dInt = a.toString() >= b.getString();
			return;
		}
	}
	if (a.typeId == Float) {
		a.typeId = Int;
		if (b.typeId == Float) {
			a.dInt = a.dFloat >= b.dFloat;
			return;
		}
		if (b.typeId == Int) {
			a.dInt = a.dFloat >= b.dInt;
			return;
		}
		if (b.typeId == String) {
			a.dInt = a.toString() >= b.getString();
			return;
		}
	}
	if (a.typeId == String) {
		a.typeId = Int;
		if (b.typeId == String) {
			a.dInt = a.getString() >= b.getString();
			return;
		}
		if (b.typeId == Int) {
			a.dInt = a.getString() >= b.toString();
			return;
		}
		if (b.typeId == Float) {
			a.dInt = a.getString() >= b.toString();
			return;
		}
	}
	FIXME("Unsupported operation %s >= %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::lessThan(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	if (a.typeId == Int) {
		if (b.typeId == Int) {
			a.dInt = a.dInt < b.dInt;
			return;
		}
		if (b.typeId == Float) {
			a.dInt = a.dInt < b.dFloat;
			return;
		}
		if (b.typeId == String) {
			a.dInt = a.toString() < b.getString();
			return;
		}
	}
	if (a.typeId == Float) {
		a.typeId = Int;
		if (b.typeId == Float) {
			a.dInt = a.dFloat < b.dFloat;
			return;
		}
		if (b.typeId == Int) {
			a.dInt = a.dFloat < b.dInt;
			return;
		}
		if (b.typeId == String) {
			a.dInt = a.toString() < b.getString();
			return;
		}
	}
	if (a.typeId == String) {
		a.typeId = Int;
		if (b.typeId == String) {
			a.dInt = a.getString() < b.getString();
			return;
		}
		if (b.typeId == Int) {
			a.dInt = a.getString() < b.toString();
			return;
		}
		if (b.typeId == Float) {
			a.dInt = a.getString() < b.toString();
			return;
		}
	}
	FIXME("Unsupported operation %s < %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::lessThanOrEqual(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	if (a.typeId == Int) {
		if (b.typeId == Int) {
			a.dInt = a.dInt <= b.dInt;
			return;
		}
		if (b.typeId == Float) {
			a.dInt = a.dInt <= b.dFloat;
			return;
		}
		if (b.typeId == String) {
			a.dInt = a.toString() <= b.getString();
			return;
		}
	}
	if (a.typeId == Float) {
		a.typeId = Int;
		if (b.typeId == Float) {
			a.dInt = a.dFloat <= b.dFloat;
			return;
		}
		if (b.typeId == Int) {
			a.dInt = a.dFloat <= b.dInt;
			return;
		}
		if (b.typeId == String) {
			a.dInt = a.toString() <= b.getString();
			return;
		}
	}
	if (a.typeId == String) {
		a.typeId = Int;
		if (b.typeId == String) {
			a.dInt = a.getString() <= b.getString();
			return;
		}
		if (b.typeId == Int) {
			a.dInt = a.getString() <= b.toString();
			return;
		}
		if (b.typeId == Float) {
			a.dInt = a.getString() <= b.toString();
			return;
		}
	}
	FIXME("Unsupported operation %s < %s", a.typeInfo().name(), b.typeInfo().name());
}

FORCEINLINE void Any::unaryMinus(VariableStack *s) {
	Any &a = s->stackArray[s->stackLevel-1];
	if (a.typeId == Int) {
		a.dInt = -a.dInt;
		return;
	}
	if (a.typeId == Float) {
		a.dFloat = -a.dFloat;
		return;
	}
	FIXME("Unsupported operation -%s", a.typeInfo().name());
}

FORCEINLINE void Any::unaryPlus(VariableStack *s) {
	Any &a = s->stackArray[s->stackLevel-1];
	if (a.typeId == Int) {
		a.dInt = +a.dInt;
		return;
	}
	if (a.typeId == Float) {
		a.dFloat = +a.dFloat;
		return;
	}
	FIXME("Unsupported operation -%s", a.typeInfo().name());
}


#endif // MATHOPERATIONS_H
