#ifndef UTIL_H
#define UTIL_H
#include "stdint.h"
#include <string>

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



#endif // UTIL_H
