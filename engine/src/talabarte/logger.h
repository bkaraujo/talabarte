#ifndef TALABARTE_LOGGER
#define TALABARTE_LOGGER
#include "talabarte/types.h"
#include "talabarte/platform/console.h"

API void logger_log(u8 level, const char* message, ...);

#ifndef FATAL
#define FATAL(msg, ...) { logger_log(0, msg, ##__VA_ARGS__); }
#endif

#ifndef ERROR
#define ERROR(msg, ...) { logger_log(1, msg, ##__VA_ARGS__); }
#endif

#ifndef WARN
#define WARN(msg, ...) { logger_log(2, msg, ##__VA_ARGS__); }
#endif 

#ifndef INFO
#define INFO(msg, ...) { logger_log(3, msg, ##__VA_ARGS__); }
#endif

#ifndef DEBUG
#define DEBUG(msg, ...) { logger_log(4, msg, ##__VA_ARGS__); }
#endif

#ifndef TRACE
#define TRACE(msg, ...) { logger_log(5, msg, ##__VA_ARGS__); }
#endif

#endif