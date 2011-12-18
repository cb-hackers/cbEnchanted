#include "precomp.h"
#include "cbenchanted.h"
#include "stringinterface.h"

StringInterface::StringInterface() : cb(static_cast <CBEnchanted *> (this)) {
	
}
		
void StringInterface::functionStr(void) {
	cb->pushValue(lexical_cast<string>(cb->popValue<int32_t>()));
}

void StringInterface::functionLeft(void) {
	int32_t n = cb->popValue<int32_t>();
	string s = cb->popValue<string>();
	
	cb->pushValue(s.substr(0, n));
}

void StringInterface::functionRight(void) {
	int32_t n = cb->popValue<int32_t>();
	string s = cb->popValue<string>();
	
	cb->pushValue(s.substr(s.length() - n));
}

void StringInterface::functionMid(void) {
	
}

void StringInterface::functionReplace(void) {
	
}

void StringInterface::functionInStr(void) {
	
}

void StringInterface::functionUpper(void) {
	
}

void StringInterface::functionLower(void) {
	
}

void StringInterface::functionTrim(void) {
	
}

void StringInterface::functionLSet(void) {
	
}

void StringInterface::functionRSet(void) {
	
}

void StringInterface::functionChr(void) {
	
}

void StringInterface::functionAsc(void) {
	
}

void StringInterface::functionLen(void) {
	
}

void StringInterface::functionHex(void) {
	
}

void StringInterface::functionBin(void) {
	
}

void StringInterface::functionString(void) {
	
}

void StringInterface::functionFlip(void) {
	
}

void StringInterface::functionStrInsert(void) {
	
}

void StringInterface::functionStrRemove(void) {
	
}

void StringInterface::functionStrMove(void) {
	
}

void StringInterface::functionCountWords(void) {
	
}

void StringInterface::functionGetWord(void) {
	
}
