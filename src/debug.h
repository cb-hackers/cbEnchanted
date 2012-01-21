#ifndef DEBUG_H
#define DEBUG_H

#ifdef LOG_LEVEL_DEBUG
#define DEBUG(str, ...) printf((string("DEBUG: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);
#else
	#define DEBUG(str, ...)
#endif

#ifdef LOG_LEVEL_INFO
#define INFO(str, ...) printf((string("INFO: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);
#else
	#define INFO(str, ...)
#endif

#ifdef LOG_LEVEL_FIXME
#define FIXME(str, ...) printf((string("FIXME: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);
#else
	#define FIXME(str, ...)
#endif

#ifdef LOG_LEVEL_STUB
#define STUB printf("FIXME: %s is stub\n", __FUNCTION__)
#else
	#define STUB
#endif

#ifdef LOG_LEVEL_HCDEBUG
#define HCDEBUG(str, ...) printf((string("HCDEBUG: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);
#else
	#define HCDEBUG(str, ...)
#endif

#endif // DEBUG_H
