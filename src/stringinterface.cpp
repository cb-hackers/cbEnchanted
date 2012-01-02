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
	string s3 = cb->popValue().getString();;
	string s2 = cb->popValue().getString();;
	string s = cb->popValue().getString();;

	string::size_type p = 0;

	while ((p = s.find(s2, p))!=string::npos)
	{
		s.replace(p, s2.size(), s3);
		++p;
	}

	cb->pushValue(s);
}

void StringInterface::functionInStr(void) {
	STUB;
}

void StringInterface::functionUpper(void) {
	STUB;
}

void StringInterface::functionLower(void) {
	STUB;
}

void StringInterface::functionTrim(void) {
	STUB;
}

void StringInterface::functionLSet(void) {
	STUB;
}

void StringInterface::functionRSet(void) {
	STUB;
}

void StringInterface::functionChr(void) {
	STUB;
}

void StringInterface::functionAsc(void) {
	STUB;
}

void StringInterface::functionLen(void) {
	string s = cb->popValue().getString();;
	cb->pushValue(int(s.length()));
}

void StringInterface::functionHex(void) {
	STUB;
}

void StringInterface::functionBin(void) {
	STUB;
}

void StringInterface::functionString(void) {
	int32_t n = cb->popValue().toInt();
	string s = cb->popValue().getString();;
	string r = s;
	for(int i = 1; i < n; i++)
	{
		r = r + s;
	}
	cb->pushValue(r);
}

void StringInterface::functionFlip(void) {
	STUB;
}

void StringInterface::functionStrInsert(void) {
	STUB;
}

void StringInterface::functionStrRemove(void) {
	STUB;
}

void StringInterface::functionStrMove(void) {
	STUB;
}

void StringInterface::functionCountWords(void) {
	STUB;
}

void StringInterface::functionGetWord(void) {
	STUB;
}
