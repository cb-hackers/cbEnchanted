#ifndef UTIL_H
#define UTIL_H
#include "stdint.h"

/**
  * Returns system timer in milliseconds
  */
int64_t mtimer();
float randf();
int32_t rand(int32_t max);

/** Returns font file location. */
std::string findfont(const char* font, bool isBold = false, bool isItalic = false);
#endif // UTIL_H
