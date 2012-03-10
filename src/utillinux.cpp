#ifdef __linux

#include "util.h"
#include <unistd.h>
#include <sys/time.h>
#include <cstdlib>
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

#endif // __linux
