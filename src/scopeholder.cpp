#include "scopeholder.h"

ScopeHolder::ScopeHolder() :
	currentScope(0),
	nextScopeIndex(0),
	scopeStackSize(10)
{
	scopes = new Scope[scopeStackSize];
}

void ScopeHolder::pushScope(int32_t byteCount, int32_t shortCount, int32_t stringCount, int32_t floatCount, int32_t integerCount, int32_t typePtrCount) {
	Scope scope;
	scope.size = byteCount +
			shortCount * 2 +
			floatCount * 4 +
			integerCount * 4 +
			stringCount * sizeof(ISString) +
			typePtrCount * sizeof(void*);
	scope.data = new char[scope.size];
	memset(scope.data, 0, scope.size);
	char *pointer(scope.data);
	scope.intVars = (int32_t*)pointer;
	pointer += integerCount * 4;
	scope.floatVars = (float*)pointer;
	pointer += floatCount * 4;
	scope.stringVars = (ISString*)pointer;
	pointer += stringCount * sizeof(ISString);
	scope.shortVars = (uint16_t*)pointer;
	pointer += shortCount * 2;
	scope.byteVars = (uint8_t*)pointer;
	pointer += byteCount;
	scope.typePtrVars = (void**)pointer;
	scope.stringVarCount = stringCount;

	if (nextScopeIndex == scopeStackSize) {
		uint32_t newSize = scopeStackSize * 2;
		Scope *newStack = new Scope[newSize];
		memcpy(newStack,scopes,scopeStackSize*sizeof(Scope));
		delete[] scopes;
		scopes = newStack;
	}
	scopes[nextScopeIndex] = scope;
	currentScope = &scopes[nextScopeIndex];
	++nextScopeIndex;
}

void ScopeHolder::popScope() {
	Scope &scope = scopes[--nextScopeIndex];
	for (int i = 0; i != scope.stringVarCount; ++i) {
		scope.stringVars[i].~ISString();
	}
	delete[] scope.data;
}
