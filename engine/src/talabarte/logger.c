#include "talabarte/core/platform/time.h"
#include "talabarte/core/platform/console.h"
#include "talabarte/memory.h"
#include "talabarte/logger.h"

#include <stdio.h>
#include <stdarg.h>

#ifndef SIZE
#define SIZE 32000
#endif

static char msg_partial[SIZE];
static char msg_final[SIZE];
static const char* strings[6] = {"FATAL", "ERROR", "WARN ", "INFO ", "DEBUG", "TRACE"};

void logger_log(u8 level, const char* message, ...) {
    LocalTime lTime = platform_time_now();
    // Technically imposes a 32k character limit on a single log entry, but...
    // DON'T DO THAT!
    memory_zero(msg_partial, SIZE);
    memory_zero(msg_final, SIZE);

    // Format original message.
    // NOTE: Oddly enough, MS's headers override the GCC/Clang va_list type with a "typedef char* va_list" in some
    // cases, and as a result throws a strange error here. The workaround for now is to just use __builtin_va_list,
    // which is the type GCC/Clang's va_start expects.
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(msg_partial, SIZE, message, arg_ptr);
    va_end(arg_ptr);

    sprintf(msg_final, "%u-%u-%u %u:%u:%u,%u %s - %s\n", 
        lTime.year, lTime.month, lTime.day, 
        lTime.hour, lTime.minute, lTime.second, lTime.millis, 
        strings[level], 
        msg_partial
    );
    platform_stdout(msg_final, level);
}