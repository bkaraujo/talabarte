#ifndef TALABARTE_LOGGER
#define TALABARTE_LOGGER
#include "talabarte/core/platform/console.h"
#include "talabarte/types.h"

API void logger_log(u8 level, const char* message, ...);

#ifndef TFATAL
#define TFATAL(msg, ...) logger_log(0, msg, ##__VA_ARGS__)
#endif

#ifndef TERROR
#define TERROR(msg, ...) logger_log(1, msg, ##__VA_ARGS__)
#endif

#ifndef TWARN
#define TWARN(msg, ...) logger_log(2, msg, ##__VA_ARGS__)
#endif 

#ifndef TINFO
#define TINFO(msg, ...) logger_log(3, msg, ##__VA_ARGS__)
#endif

#ifndef TDEBUG
#define TDEBUG(msg, ...) logger_log(4, msg, ##__VA_ARGS__)
#endif

#ifndef TTRACE
#define TTRACE(msg, ...) logger_log(5, msg, ##__VA_ARGS__)
#endif

#endif