#ifndef CBDLL_H
#define CBDLL_H

#include "precomp.h"
#include "isstring.h"


class CBDLL
{
	public:
#ifdef _WIN32
		typedef void (*CBDLLFunction)(const void *, int32_t, void *, int32_t);
#else
		typedef void (*CBDLLFunction)(const void *, int32_t, void *, int32_t);
#endif
		typedef unordered_map<string, CBDLLFunction> FunctionMap;
		CBDLL();
		bool load(const ISString &f);
		void call(const string &name, const void *in, int32_t inSize, void *out, int32_t outSize);

	private:
		FunctionMap functions;
		void *dll;
};

#endif // CBDLL_H
