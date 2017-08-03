#include "util.h"
#include <chrono>
#include <random>


#if (defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64))
#include <windows.h>
inline std::chrono::milliseconds UpTime() { return std::chrono::milliseconds(GetTickCount()); }
#elif defined(__linux__)
#include <sys/sysinfo.h>
std::chrono::milliseconds UpTime() {
	std::chrono::milliseconds uptime(0u);
	struct sysinfo x;
	if (!sysinfo(&x))
		uptime = std::chrono::milliseconds((uint64_t)(x.uptime) * 1000ULL);
	return std::move(uptime);
}
#elif defined(__APPLE__) && defined(__MACH__)
#include <time.h>
#include <errno.h>
#include <sys/sysctl.h>

std::chrono::milliseconds UpTime() {
	std::chrono::milliseconds uptime(0u);
	struct timeval tval;
	size_t len = sizeof(tval);
	int mib[2] = { CTL_KERN, KERN_BOOTTIME };
	if (!sysctl(mib, 2, &tval, &len, NULL, 0) == 0) {
		uptime = std::chrono::milliseconds((uint64_t)(tval.tv_sec) * 1000ULL + (uint64_t)(tval.tv_usec) / 1000ULL);
	}
	return std::move(uptime);
}

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#include <time.h>
std::chrono::milliseconds UpTime() {
	std::chrono::milliseconds uptime(0u);
	struct timespec ts;
	if (!clock_gettime(CLOCK_UPTIME_PRECISE, &ts))
		uptime = std::chrono::milliseconds((uint64_t)(ts.tv_sec) * 1000ULL + (uint64_t)(ts.tv_nsec) / 1000000ULL);
	return std::move(uptime);
}
#endif



//NOTE(everyone): Standard library is a hot mess, so these type aliasses are in good use. :)
using HighresolutionClock = std::chrono::high_resolution_clock;
using Timepoint = std::chrono::time_point<HighresolutionClock>;

//NOTE(everyone): These static objects will be initialized along with the program. 
static std::chrono::milliseconds gUptime = UpTime();
static Timepoint gExecutionStarts(HighresolutionClock::now());
static std::mt19937 gRandomEngine;


#define CastDuration( __DURATION__ ) std::chrono::duration_cast<std::chrono::milliseconds>( __DURATION__ )

int64_t mtimer() {
	return CastDuration(gUptime + (HighresolutionClock::now() - gExecutionStarts)).count();
}

/*
int64_t sinceStart() {
	return CastDuration(HighresolutionClock::now() - gExecutionStarts).count();
}
*/

#undef CastDuration


void randomize(int32_t seed) {
	gRandomEngine.seed(seed);
}

float randf() {
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	return distribution(gRandomEngine);
}

int32_t rand(int32_t max) {
	std::uniform_int_distribution<int32_t> distribution(0, max);
	return distribution(gRandomEngine);
}
