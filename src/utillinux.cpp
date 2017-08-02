#if defined(__linux) || defined(__APPLE__)

#include "util.h"
#ifdef FONTCONFIG_FOUND
#include <fontconfig.h>
#endif // FONTCONFIG_FOUND
#include <unistd.h>
#include <sys/time.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/locale.hpp>
#include <dlfcn.h>

using namespace std;
using namespace boost::locale::conv;

#if 0

int64_t mtimer() {
	struct timeval tv;

	gettimeofday(&tv, NULL);

	//mtime = (tiemz.tv_sec * 1000 + tiemz.tv_usec / 1000.0) + 0.5;
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}



float randf() {
	return (float)rand() / RAND_MAX;
}

int32_t rand(int32_t max) {
	return rand() % max;
}
#endif 

string findfont(const char* font, bool isBold, bool isItalic) {
#ifdef FONTCONFIG_FOUND
	string fontpath = "";
	// Create a font query
	cout << "Trying to find font " << font;
	if (isBold) cout << " [BOLD]";
	if (isItalic) cout << " [ITALIC]";
	cout << endl;
	FcPattern* pattern = FcPatternBuild(NULL,
	                                    FC_FAMILY, FcTypeString, font,
	                                    FC_WEIGHT, FcTypeInteger, isBold ? FC_WEIGHT_BOLD : FC_WEIGHT_REGULAR,
	                                    FC_SLANT, FcTypeInteger, isItalic ? FC_SLANT_ITALIC : FC_SLANT_ROMAN,
	                                    NULL);
	//DEBUG FcPatternPrint(pattern);

	// Add default initialization values to the pattern
	FcDefaultSubstitute(pattern);
	//DEBUG FcPatternPrint(pattern);

	// Find the best font
	FcResult result;
	FcPattern* matched = FcFontMatch(NULL, pattern, &result);
	//DEBUG FcPatternPrint(matched);

	// Did we find that font?
	if (matched) {
		// Oh yeah.
		FcChar8* fontconfig_path;
		FcPatternGetString(matched, FC_FILE, 0, &fontconfig_path);
		fontpath = string((char*)fontconfig_path);
		cout << " -> Best match: \"" << fontpath.c_str() << "\"" << endl;
	}
	else {
		// Uh oh, there's no such font. We can only error out in textinterface.cpp for this :(
		cout << " -> font not found :(" << endl;
	}

	// Don't leak memory
	FcPatternDestroy(pattern);
	FcPatternDestroy(matched);

	return fontpath;
#else
	return "";
#endif // FONTCONFIG_FOUND
}

/** Converts UTF-8 to Windows-1252, returns the converted string. */
string utf8toCP1252(string str) {
	string newStr = from_utf(str, "windows-1252");
	return newStr;
}

/** Converts Windows-1252 to UTF-8, returns the converted string. */
string CP1252toUtf8(string str) {
	string newStr = to_utf<char>(str, "windows-1252");
	return newStr;
}

void *loadDLL(const ISString &f) {
	return dlopen (f.getUtf8Encoded().c_str(), RTLD_LAZY);
}

void *getDLLFunction(void *dll, const string &s) {
	return dlsym(dll, s.c_str());
}

#endif // defined(__linux) || defined(__APPLE__)
