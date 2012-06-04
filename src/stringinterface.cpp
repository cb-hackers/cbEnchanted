#include <bitset>
#include "precomp.h"
#include "cbenchanted.h"
#include "stringinterface.h"
#include "errorsystem.h"

StringInterface::StringInterface() {
	cb = static_cast <CBEnchanted *> (this);
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

	if (n < 0) {
		cb->errors->createError("Right() failed!", "Second parameter must be positive");
		cb->pushValue(ISString(""));
		return;
	}

	string::size_type uN = n;
	if (uN >= s.length()) {
		cb->pushValue(s);
	} else {
		cb->pushValue(s.substr(s.length() - uN));
	}
}

void StringInterface::functionMid(void) {
	int32_t len = cb->popValue().getInt();
	int32_t pos = cb->popValue().getInt();
	string str = cb->popValue().getString().getRef();

	if (pos <= 0) {
		cb->errors->createError("Mid() failed!", "Second parameter must be greater than 0");
		cb->pushValue(ISString(""));
		return;
	}

	if (pos < 0) {
		// Return remainder of string
		cb->pushValue(str.substr(pos - 1));
	}
	else {
		string::size_type uPos = pos;
		if (uPos - 1 > str.length()) {
			cb->pushValue(ISString(""));
		}
		else {
			cb->pushValue(str.substr(uPos - 1, len));
		}
	}
}

void StringInterface::functionReplace(void) {
	string s3 = cb->popValue().getString().getRef();
	string s2 = cb->popValue().getString().getRef();
	string s = cb->popValue().getString().getRef();

	if(s2.empty()) {
		cb->pushValue(string(s));
		return;
	}

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

	if (!str.empty()) {
		boost::algorithm::trim(str);
	}

	cb->pushValue(str);
}

void StringInterface::functionLSet(void) {
	int32_t len = cb->popValue().getInt();
	string str = cb->popValue().getString().getRef();

	if (len < 0) {
		cb->errors->createError("LSet() failed!", "Second parameter must be positive");
		cb->pushValue(ISString(""));
		return;
	}

	str.resize(len, ' ');
	cb->pushValue(str);
}

void StringInterface::functionRSet(void) {
	int32_t len = cb->popValue().getInt();
	string str = cb->popValue().getString().getRef();

	if (len < 0) {
		cb->errors->createError("RSet() failed!", "Second parameter must be positive");
		cb->pushValue(ISString(""));
		return;
	}

	string::size_type uLen = len;
	if (uLen > str.length()) {
		string str2;
		str2.resize(uLen - str.length(), ' ');
		cb->pushValue(str2 + str);
	}
	else {
		cb->pushValue(str.substr(str.length() - uLen));
	}
}

void StringInterface::functionChr(void) {
	unsigned char AV = cb->popValue().getInt();
	string s(1, AV);
	cb->pushValue(s);
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

	bitset<32> bs(V);

	cb->pushValue(bs.to_string());
}

void StringInterface::functionString(void) {
	int32_t n = cb->popValue().toInt();
	string s = cb->popValue().getString().getRef();
	string r = s;
	for(int i = 1; i < n; i++) {
		r = r + s;
	}
	cb->pushValue(r);
}

void StringInterface::functionFlip(void) {
	string str = cb->popValue().getString().getRef();
	stringstream ss;
	string::const_reverse_iterator rit;
	for (rit = str.rbegin(); rit < str.rend(); rit++) {
		ss << *rit;
	}

	cb->pushValue(ss.str());
}

void StringInterface::functionStrInsert(void) {
	string txt = cb->popValue().getString().getRef();
	int32_t pos = cb->popValue().getInt();
	string str = cb->popValue().getString().getRef();

	if (pos < 0) {
		cb->errors->createError("StrInsert() failed!", "Second parameter must be positive");
		cb->pushValue(ISString(""));
		return;
	}

	string::size_type uPos = pos;
	if (uPos > str.length()) {
		str.insert(str.length(), txt);
	}
	else {
		str.insert(uPos, txt);
	}

	cb->pushValue(str);

}

void StringInterface::functionStrRemove(void) {
	int32_t len = cb->popValue().getInt();
	int32_t pos = cb->popValue().getInt();
	string str = cb->popValue().getString().getRef();

	if (pos <= 0) {
		cb->errors->createError("StrRemove() failed!", "Second parameter must be greater than 0");
		cb->pushValue(ISString(""));
		return;
	}
	if (len < 0) {
		cb->errors->createError("StrRemove() failed!", "Third parameter must be positive");
		cb->pushValue(ISString(""));
		return;
	}

	string::size_type uLen = len;
	string::size_type uPos = pos;

	if (uPos < str.length()) {
		if(uPos - 1 + uLen < str.length()) {
			str.erase(uPos - 1, uLen);
		}
		else {
			str.erase(uPos - 1, str.length());
		}
	}

	cb->pushValue(str);

}

void StringInterface::functionStrMove(void) {
	int32_t mov = cb->popValue().getInt();
	int32_t len = cb->popValue().getInt();
	int32_t pos = cb->popValue().getInt();
	string str = cb->popValue().getString().getRef();

	if (pos <= 0) {
		cb->errors->createError("StrMove() failed!", "Second parameter must be greater than 0");
		cb->pushValue(ISString(""));
		return;
	}
	if (len < 0) {
		cb->errors->createError("StrMove() failed!", "Third parameter must be positive");
		cb->pushValue(ISString(""));
		return;
	}
	if (mov < 0) {
		cb->errors->createError("StrMove() failed!", "Fourth parameter must be positive");
		cb->pushValue(ISString(""));
		return;
	}

	string::size_type uMov = mov;
	string::size_type uLen = len;
	string::size_type uPos = pos;

	if(uPos - 1 + uLen > str.length()) {
		cb->pushValue(str);
		return;
	}

	string txt = str.substr(uPos - 1, uLen);
	str.erase(uPos - 1, uLen);

	if (-1 + uPos + uMov > str.length()) {
		str.insert(str.length(), txt);
	}
	else {
		str.insert(-1 + uPos + uMov, txt);
	}

	cb->pushValue(str);
}

void StringInterface::functionCountWords(void) {
	string sep = cb->popValue().toString().getRef();
	string str = cb->popValue().toString().getRef();

	if (str.empty()) {
		cb->pushValue(0);
		return;
	}
	if (sep.empty()) {
		sep = ' ';
	}

	string::size_type p = 0;
	int32_t count = 1;

	while ((p = str.find(sep, p)) != string::npos) {
		++p;
		++count;
	}

	cb->pushValue(count);
}

void StringInterface::functionGetWord(void) {
	string sep = cb->popValue().toString().getRef();
	int32_t w = cb->popValue().toInt();
	string str = cb->popValue().toString().getRef();

	if (sep.empty()) {
		sep = ' ';
	}

	int32_t sep_pos = 0;

	for (int i = 1; i < w; ++i) {
		sep_pos = str.find(sep);
		if (sep_pos) {
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
