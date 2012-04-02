#ifdef _WIN32

#include "util.h"
#include <time.h>
#include <cstdlib>
int64_t mtimer() {
	return clock() * 1000 / CLOCKS_PER_SEC;
}


float randf() {
	return (float)rand() / RAND_MAX; //TODO: Better generation (Windows RAND_MAX == 0x7fff)
}

int32_t rand(int32_t max) {
	return rand() % (max + 1); //TODO: Better generation (Windows RAND_MAX == 0x7fff)
}

#endif // _WIN32
