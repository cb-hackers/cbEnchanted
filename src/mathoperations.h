#ifndef MATHOPERATIONS_H
#define MATHOPERATIONS_H
#include "any.h"
#include "variablestack.h"
FORCEINLINE void Any::addition(VariableStack *s) {
	Any &b = s->stackArray[--s->stackLevel];
	Any &a = s->stackArray[s->stackLevel-1];
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
			a = a.dString->str + boost::lexical_cast<string>(b.dFloat);
			return;
		}
	}
	if (b.typeId == Int) {
		if (a.typeId == Float) {
			a.dFloat = a.dFloat + b.dInt;
			return;
		}
		if (a.typeId == Int) {
			a.dInt = a.dInt + b.dInt;
			return;
		}
		if (a.typeId == String) {
			a = a.dString->str + boost::lexical_cast<string>(b.dInt);
			return;
		}
	}
	if (b.typeId == String) {
		if (a.typeId == Float) {
			a = boost::lexical_cast<string>(a.dFloat) + b.dString->str;
			return;
		}
		if (a.typeId == Int) {
			a = boost::lexical_cast<string>(a.dInt) + b.dString->str;
			return;
		}
		if (a.typeId == String) {
			a = a.dString->str + b.dString->str;
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

FORCEINLINE void Any::substraction(VariableStack *s) {
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

#endif // MATHOPERATIONS_H
