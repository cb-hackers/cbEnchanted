#include "precomp.h"
#include "cbenchanted.h"
#include "stringinterface.h"

StringInterface::StringInterface() {
	cb = static_cast <CBEnchanted *> (this);
}

void StringInterface::functionStr(void) {
	Any a = cb->popValue();
	switch (a.type()) {
		case Any::Int:
			cb->pushValue(ISString::fromInt(a.getInt()));
			return;
		case Any::Float:
			cb->pushValue(ISString::fromFloat(a.getFloat()));
			return;
		default:
			cb->pushValue(a);
	}
}

void StringInterface::functionLeft(void) {
	int32_t n = cb->popValue().toInt();
	const ISString &s = cb->popValue().getString();

	cb->pushValue(s.left(n));
}

void StringInterface::functionRight(void) {
	int32_t n = cb->popValue().toInt();
	const ISString &s = cb->popValue().getString();

	cb->pushValue(s.right(n));
}

void StringInterface::functionMid(void) {
	int32_t len = cb->popValue().getInt();
	int32_t pos = cb->popValue().getInt();
	const ISString &str = cb->popValue().getString();

	cb->pushValue(str.mid(pos, len));
}

void StringInterface::functionReplace(void) {
	const ISString &s3 = cb->popValue().getString();
	const ISString &s2 = cb->popValue().getString();
	const ISString &s = cb->popValue().getString();

	cb->pushValue(s.replace(s2, s3));
}

void StringInterface::functionInStr(void) {
	int32_t pos = cb->popValue().getInt();
	const ISString &sep = cb->popValue().getString();
	const ISString &str = cb->popValue().getString();

	cb->pushValue(str.inStr(sep,pos));
}

void StringInterface::functionUpper(void) {
	const ISString &str = cb->popValue().getString();
	cb->pushValue(str.toUpper());
}

void StringInterface::functionLower(void) {
	const ISString &str = cb->popValue().getString();
	cb->pushValue(str.toLower());
}

void StringInterface::functionTrim(void) {
	const ISString &str = cb->popValue().getString();
	cb->pushValue(str.trim());
}

void StringInterface::functionLSet(void) {
	int32_t len = cb->popValue().getInt();
	string str = cb->popValue().getString().toStdString();

	str.resize(len, ' ');
	cb->pushValue(str);
}

void StringInterface::functionRSet(void) {
	int32_t len = cb->popValue().getInt();
	string str = cb->popValue().getString().toStdString();

	if(len>str.length()) {
		string str2;
		str2.resize(len - str.length(), ' ');
		cb->pushValue(str2 + str);
	} else {
		cb->pushValue(str.substr(str.length()-len));
	}
}

void StringInterface::functionChr(void) {
	unsigned char s[2];
	//ISString constructor automatically encodes char* to ALLEGRO_USTR.
	s[0] = cb->popValue().getInt();
	s[1] = 0;
	cb->pushValue(ISString((char*)s));
}

void StringInterface::functionAsc(void) {
	//TODO
	uint8_t AV = uint8_t(*cb->popValue().getString().cString());
	cb->pushValue(int(AV));
}

void StringInterface::functionLen(void) {
	const ISString &s = cb->popValue().getString();
	cb->pushValue(int32_t(s.length()));
}

void StringInterface::functionHex(void) {
	int32_t V = cb->popValue().getInt();
	stringstream ss;

	ss << hex << V;

	string str1 = ss.str();
	string str2;
	str2.resize(8 - str1.length(), '0');
	cb->pushValue(str2 + boost::algorithm::to_upper_copy(str1));
}

void StringInterface::functionBin(void) {
	int32_t V = cb->popValue().getInt();
	stringstream ss;

	const boost::dynamic_bitset<> bin(32, V);
	ss << bin;

	cb->pushValue(ss.str());
}

void StringInterface::functionString(void) {
	int32_t n = cb->popValue().toInt();
	ISString s = cb->popValue().getString();
	ISString r = s;
	for(int i = 1; i < n; i++)
	{
		r += s;
	}
	cb->pushValue(r);
}

void StringInterface::functionFlip(void) {
	//TODO: Fix this
	string str = cb->popValue().getString().toStdString();
	stringstream ss;
	string::const_reverse_iterator rit;
	for (rit = str.rbegin();rit < str.rend(); rit++)
	{
		ss << *rit;
	}

	cb->pushValue(ss.str());

}

void StringInterface::functionStrInsert(void) {
	//TODO: Fix this
	string txt = cb->popValue().getString().toStdString();
	int32_t pos = cb->popValue().getInt();
	string str = cb->popValue().getString().toStdString();

	str.insert(pos, txt);

	cb->pushValue(str);

}

void StringInterface::functionStrRemove(void) {
	int32_t len = cb->popValue().getInt();
	int32_t pos = cb->popValue().getInt();
	string str = cb->popValue().getString().toStdString();

	str.erase(pos-1, len);

	cb->pushValue(str);

}

void StringInterface::functionStrMove(void) {
	int32_t mov = cb->popValue().getInt();
	int32_t len = cb->popValue().getInt();
	int32_t pos = cb->popValue().getInt();
	string str = cb->popValue().getString().toStdString();

	string txt = str.substr(pos-1, len);
	str.erase(pos-1, len);

	str.insert(-1+pos+mov, txt);

	cb->pushValue(str);
}

void StringInterface::functionCountWords(void) {
	string sep = cb->popValue().toString().toStdString();
	string str = cb->popValue().toString().toStdString();

	if(str=="") {
		cb->pushValue(0);
		return;
	}
	if(sep=="") {sep = ' ';}

	string::size_type p = 0;
	int32_t count = 1;

	while ((p = str.find(sep, p))!=string::npos)
	{
		++p;
		++count;
	}

	cb->pushValue(count);
}

void StringInterface::functionGetWord(void) {
	string sep = cb->popValue().toString().toStdString();
	int32_t w = cb->popValue().toInt();
	string str = cb->popValue().toString().toStdString();

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
