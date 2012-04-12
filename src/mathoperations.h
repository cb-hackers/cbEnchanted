#ifndef MATHOPERATIONS_H
#define MATHOPERATIONS_H
#include "any.h"
#include "variablestack.h"
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
			a.dFloat = a.dInt / b.dFloat;
			a.typeId = Float;
			return;
		}
	}
	if (b.typeId == Int) {
		if (a.typeId == Float) {
			a.dFloat = a.dFloat / b.dInt;
			return;
		}
		if (a.typeId == Int) {
			a.dInt = a.dInt / b.dInt;
			return;
		}
	}
	FIXME("Unsupported operation %s * %s", a.typeInfo().name(), b.typeInfo().name());
}

void Any::modulo(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	a.dInt = a.toInt() % b.toInt();
	a.typeId = Int;
}

void Any::shl(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	a.dInt = a.toInt() << b.toInt();
	a.typeId = Int;
}

void Any::sar(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
	a.dInt = a.toInt() >> b.toInt();
	a.typeId = Int;
}

void Any::shr(VariableStack *s) {
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




#endif // MATHOPERATIONS_H
