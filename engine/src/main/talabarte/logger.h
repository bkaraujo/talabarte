#ifndef TALABARTE_LOGGER
#define TALABARTE_LOGGER
#include "talabarte/core/platform/detector.h"
#include "talabarte/core/platform/console.h"
#include "talabarte/types.h"
#include <stdlib.h>
#include <string.h>

#ifdef TALABARTE_PLATFORM_WINDOWS
#define __FILENAME__ (strrchr(__BASE_FILE__, '\\') ? strrchr(__BASE_FILE__, '\\') + 1 : __BASE_FILE__)
#else
#define __FILENAME__ (strrchr(__BASE_FILE__, '/') ? strrchr(__BASE_FILE__, '/') + 1 : __BASE_FILE__)
#endif

API void logger_log(u8 level, const char* filename, u32 line, const char* message, ...);

#ifndef TFATAL
#define TFATAL(msg, ...) logger_log(0, __FILENAME__, __LINE__, msg, ##__VA_ARGS__); exit(9)
#endif

#ifndef TERROR
#define TERROR(msg, ...) logger_log(1, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#endif

#ifndef TWARN
#define TWARN(msg, ...) logger_log(2, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#endif 

#ifndef TINFO
#define TINFO(msg, ...) logger_log(3, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#endif

#ifndef TDEBUG
#define TDEBUG(msg, ...) logger_log(4, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#endif

#ifndef TTRACE
#define TTRACE(msg, ...) logger_log(5, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#endif

#endif