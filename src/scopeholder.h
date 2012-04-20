#ifndef VARIABLESCOPEHOLDER_H
#define VARIABLESCOPEHOLDER_H
#include "precomp.h"
#include "isstring.h"

class ScopeHolder {
	private:
		struct Scope {
				int32_t *intVars;
				float *floatVars;
				uint16_t *shortVars;
				uint8_t *byteVars;
				ISString *stringVars;
				void **typePtrVars;
				char *data;
				uint32_t size;
				uint32_t stringVarCount;
		};
	public:
		ScopeHolder();
		void pushScope(int32_t byteCount,int32_t shortCount,int32_t stringCount,int32_t floatCount,int32_t integerCount,int32_t typePtrCount);
		void popScope();
		FORCEINLINE int32_t &getIntVar(int32_t i) const {return currentScope->intVars[i-1];}
		FORCEINLINE float &getFloatVar(int32_t i) const {return currentScope->floatVars[i-1];}
		FORCEINLINE uint16_t &getShortVar(int32_t i) const {return currentScope->shortVars[i-1];}
		FORCEINLINE uint8_t &getByteVar(int32_t i) const {return currentScope->byteVars[i-1];}
		FORCEINLINE ISString &getStringVar(int32_t i) const {return currentScope->stringVars[i-1];}
		FORCEINLINE void *&getTypePtrVar(int32_t i) const {return currentScope->typePtrVars[i-1];}

		FORCEINLINE void setIntVar(int32_t i, int32_t v) const {currentScope->intVars[i-1] = v;}
		FORCEINLINE void setFloatVar(int32_t i, float v) const {currentScope->floatVars[i-1] = v;}
		FORCEINLINE void setShortVar(int32_t i, uint16_t v) const {currentScope->shortVars[i-1] = v;}
		FORCEINLINE void setByteVar(int32_t i, uint8_t v) const {currentScope->byteVars[i-1] = v;}
		FORCEINLINE void setStringVar(int32_t i, const ISString &v) const {currentScope->stringVars[i-1] = v;}
		FORCEINLINE void setTypePtrVar(int32_t i,void *v) const {currentScope->typePtrVars[i-1] = v;}
		int32_t depth() const {return nextScopeIndex;}
	private:
		Scope *scopes;
		uint32_t scopeStackSize;
		Scope *currentScope;
		uint32_t nextScopeIndex;
};

#endif // VARIABLESCOPE_H
