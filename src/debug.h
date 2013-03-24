#ifndef DEBUG_H
#define DEBUG_H

#ifdef LOG_LEVEL_DEBUG
#define DEBUG(str, ...) printf((string("DEBUG: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);debug_breakpoint_place();
#else
	#define DEBUG(str, ...)
#endif

#ifdef LOG_LEVEL_INFO
#define INFO(str, ...) printf((string("INFO: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);info_breakpoint_place();
#else
	#define INFO(str, ...)
#endif

#ifdef LOG_LEVEL_FIXME
#define FIXME(str, ...) printf((string("FIXME: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);fixme_breakpoint_place();
#else
	#define FIXME(str, ...)
#endif

#ifdef LOG_LEVEL_STUB
#define STUB printf("FIXME: %s is stub\n", __FUNCTION__);stub_breakpoint_place();
#else
	#define STUB
#endif

#ifdef LOG_LEVEL_HCDEBUG
#define HCDEBUG(str, ...) printf((string("HCDEBUG: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);hcdebug_breakpoint_place();
#else
	#define HCDEBUG(str, ...)
#endif

#define FATAL_ERROR(str, ... ) printf((string("ERROR: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);fatalError();

void hcdebug_breakpoint_place();
void stub_breakpoint_place();
void fixme_breakpoint_place();
void info_breakpoint_place();
void debug_breakpoint_place();
void fatalError();

#endif // DEBUG_H
