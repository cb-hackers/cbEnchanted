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
	string sep = cb->popValue().toString();
	string str = cb->popValue().toString();

	if(sep=="") {sep = ' ';}

	string::size_type p = 0;
	int32_t count = 0;

	while ((p = str.find(sep, p))!=string::npos)
	{
		++p;
		++count;
	}

	count++;

	cb->pushValue(count);

}

void StringInterface::functionGetWord(void) {
	string sep = cb->popValue().toString();
	int32_t w = cb->popValue().toInt();
	string str = cb->popValue().toString();

	if(sep=="") {sep = ' ';}

	int32_t sep_pos = 0;

	for(int i = 1; i != w; ++i)
	{
		sep_pos = str.find(sep);
		if(sep_pos) {
			str = str.substr(sep_pos + 1);
		}
	}

	sep_pos = str.find(sep);
	if (sep_pos) {
		cb->pushValue(str.substr(0, sep_pos));
	} else {
		cb->pushValue(str.substr(0));
	}

}
