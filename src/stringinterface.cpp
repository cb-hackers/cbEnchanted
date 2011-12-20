#include "precomp.h"
#include "cbenchanted.h"
#include "stringinterface.h"

StringInterface::StringInterface() : cb(static_cast <CBEnchanted *> (this)) {
	
}
		
void StringInterface::functionStr(void) {
    Any a = cb->popValue();
    switch (a.type())
    {
    case Any::Int: cb->pushValue(lexical_cast<string>(a.getInt()));return;
    case Any::Float: cb->pushValue(lexical_cast<string>(a.getFloat()));return;
    case Any::Short: cb->pushValue(lexical_cast<string>(a.getShort()));return;
    case Any::Byte: cb->pushValue(lexical_cast<string>(a.getByte()));return;
    default:cb->pushValue(a);
    }


}

void StringInterface::functionLeft(void) {
    int32_t n = cb->popValue().toInt();
    string s = cb->popValue().getString();
	
	cb->pushValue(s.substr(0, n));
}

void StringInterface::functionRight(void) {
    int32_t n = cb->popValue().toInt();
    string s = cb->popValue().getString();;
	
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
