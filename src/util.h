#ifndef UTIL_H
#define UTIL_H
#include "stdint.h"
#include <string>
#include "isstring.h"

/**
  * Returns system timer in milliseconds
  */
int64_t mtimer();

/** Returns random float between 0.0 and 1.0
  */
float randf();

/** Returns random integer between 0 - max
  */
int32_t rand(int32_t max);

/** Returns font file location. */
std::string findfont(const char* font, bool isBold = false, bool isItalic = false);

/** Converts UTF-8 to Windows-1252, returns the converted string. */
std::string utf8toCP1252(std::string str);

/** Converts Windows-1252 to UTF-8, returns the converted string. */
std::string CP1252toUtf8(std::string str);

#ifdef _WIN32
	/** Returns the given string as a UTF-16 encoded wstring */
	std::wstring utf8ToUtf16(std::string str);
#endif

/** Loads dynamic link library and returns handle to it */
void *loadDLL(const ISString &f);

/** Returns function pointer from dll with given name */
void *getDLLFunction(void *dll, const string &s);


#endif // UTIL_H
