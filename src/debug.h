#ifndef DEBUG_H
#define DEBUG_H

#define LOG_DEBUG		0x01
#define LOG_INFO		0x02
#define LOG_FIXME		0x04
#define LOG_HCDEBUG		0x08

#define LOG_LEVEL (LOG_DEBUG | LOG_INFO | LOG_FIXME)

#if LOG_LEVEL & 1
#define DEBUG(str, ...) printf((string("DEBUG: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);
#else
    #define DEBUG(str, ...)
#endif

#if (LOG_LEVEL >> 1) & 1
#define INFO(str, ...) printf((string("INFO: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);
#else
    #define INFO(str, ...)
#endif

#if (LOG_LEVEL >> 2) & 1
#define FIXME(str, ...) printf((string("FIXME: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);
#else
    #define FIXME(str, ...)
#endif

#if (LOG_LEVEL >> 3) & 1
#define HCDEBUG(str, ...) printf((string("HCDEBUG: ") + string(str) + string("\n")).c_str(), ##__VA_ARGS__);
#else
    #define HCDEBUG(str, ...)
#endif

#endif // DEBUG_H
