#ifdef __linux

#include "util.h"
#ifdef FONTCONFIG_FOUND
#include <fontconfig.h>
#endif // FONTCONFIG_FOUND
#include <unistd.h>
#include <sys/time.h>
#include <cstdlib>
#include <iostream>

using std::cout;
using std::endl;

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

char* findfont(const char* font, bool isBold, bool isItalic) {
	char* fontpath = "";
#ifdef FONTCONFIG_FOUND
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
	
	// Do default elimination and initialization for the pattern
	FcDefaultSubstitute(pattern);
	//DEBUG FcPatternPrint(pattern);
	
	// Find the best font
	FcResult result;
	FcPattern* matched = FcFontMatch(NULL, pattern, &result);
	//DEBUG FcPatternPrint(matched);
	
	// Did we find that font?
	if (matched) {
		// Oh yeah.
		FcPatternGetString(matched, FC_FILE, 0, &fontpath);
		cout << " -> Best match: \"" << fontpath << "\"" << endl;
	}
	else {
		// Uh oh, there's no such font. We can only error out in textinterface.cpp for this :(
		cout << " -> font not found :(" << endl;
	}
#endif // FONTCONFIG_FOUND
	return fontpath;
}

#endif // __linux
