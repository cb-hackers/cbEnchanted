#include "precomp.h"
#include "interfacesystem.h"
#include "stringinterface.h"

StringInterface::StringInterface() {
	sys = static_cast <InterfaceSystem *> (this);
}

void StringInterface::functionStr(void) {
	Any a = cb->popValue();
	switch (a.type()) {
		case Any::Int:
			cb->pushValue(lexical_cast<string>(a.getInt()));
		return;
		case Any::Float:
			cb->pushValue(lexical_cast<string>(a.getFloat()));
		return;
		default:
			cb->pushValue(a);
	}
}

void StringInterface::functionLeft(void) {
	int32_t n = cb->popValue().toInt();
	string s = cb->popValue().getString().getRef();

	cb->pushValue(s.substr(0, n));
}

void StringInterface::functionRight(void) {
	int32_t n = cb->popValue().toInt();
	string s = cb->popValue().getString().getRef();

	cb->pushValue(s.substr(s.length() - n));
}

void StringInterface::functionMid(void) {
	int32_t len = cb->popValue().getInt();
	int32_t pos = cb->popValue().getInt();
	string str = cb->popValue().getString().getRef();

	cb->pushValue(str.substr(pos-1, len));
}

void StringInterface::functionReplace(void) {
	string s3 = cb->popValue().getString().getRef();
	string s2 = cb->popValue().getString().getRef();
	string s = cb->popValue().getString().getRef();

	string::size_type p = 0;

	while ((p = s.find(s2, p))!=string::npos)
	{
		s.replace(p, s2.size(), s3);
		++p;
	}

	cb->pushValue(s);
}

void StringInterface::functionInStr(void) {
	int32_t pos = cb->popValue().getInt();
	string sep = cb->popValue().getString().getRef();
	string str = cb->popValue().getString().getRef();

	cb->pushValue(int32_t(str.find(sep, pos-1)+1));
}

void StringInterface::functionUpper(void) {
	string str = cb->popValue().getString().getRef();
	cb->pushValue(boost::algorithm::to_upper_copy(str));
}

void StringInterface::functionLower(void) {
	string str = cb->popValue().getString().getRef();

	cb->pushValue(boost::algorithm::to_lower_copy(str));
}

void StringInterface::functionTrim(void) {
	string str = cb->popValue().getString().getRef();

	str = str.substr(str.find_first_not_of(" "));
	str = str.substr(0, str.find_last_not_of(" ")+1);

	cb->pushValue(str);
}

void StringInterface::functionLSet(void) {
	int32_t len = cb->popValue().getInt();
	string str = cb->popValue().getString().getRef();

	str.resize(len, ' ');
	cb->pushValue(str);
}

void StringInterface::functionRSet(void) {
	int32_t len = cb->popValue().getInt();
	string str = cb->popValue().getString().getRef();

	if(len>str.length()) {
		string str2;
		str2.resize(len - str.length(), ' ');
		cb->pushValue(str2 + str);
	} else {
		cb->pushValue(str.substr(str.length()-len));
	}
}

void StringInterface::functionChr(void) {
	unsigned char AV = cb->popValue().getInt();
	char buffer[2];
	buffer[0] = *(char*)&AV;
	buffer[1] = 0;
	cb->pushValue(buffer);
}

void StringInterface::functionAsc(void) {
	uint8_t AV = uint8_t(*cb->popValue().getString().getRef().c_str());
	cb->pushValue(int(AV));
}

void StringInterface::functionLen(void) {
	string s = cb->popValue().getString().getRef();
	cb->pushValue(int(s.length()));
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
	string s = cb->popValue().getString().getRef();
	string r = s;
	for(int i = 1; i < n; i++)
	{
		r = r + s;
	}
	cb->pushValue(r);
}

void StringInterface::functionFlip(void) {
	string str = cb->popValue().getString().getRef();
	stringstream ss;
	string::const_reverse_iterator rit;
	for (rit = str.rbegin();rit < str.rend(); rit++)
	{
		ss << *rit;
	}

	cb->pushValue(ss.str());

}

void StringInterface::functionStrInsert(void) {
	string txt = cb->popValue().getString().getRef();
	int32_t pos = cb->popValue().getInt();
	string str = cb->popValue().getString().getRef();

	str.insert(pos, txt);

	cb->pushValue(str);

}

void StringInterface::functionStrRemove(void) {
	int32_t len = cb->popValue().getInt();
	int32_t pos = cb->popValue().getInt();
	string str = cb->popValue().getString().getRef();

	str.erase(pos-1, len);

	cb->pushValue(str);

}

void StringInterface::functionStrMove(void) {
	int32_t mov = cb->popValue().getInt();
	int32_t len = cb->popValue().getInt();
	int32_t pos = cb->popValue().getInt();
	string str = cb->popValue().getString().getRef();

	string txt = str.substr(pos-1, len);
	str.erase(pos-1, len);

	str.insert(-1+pos+mov, txt);

	cb->pushValue(str);
}

void StringInterface::functionCountWords(void) {
	string sep = cb->popValue().toString().getRef();
	string str = cb->popValue().toString().getRef();

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
	string sep = cb->popValue().toString().getRef();
	int32_t w = cb->popValue().toInt();
	string str = cb->popValue().toString().getRef();

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
