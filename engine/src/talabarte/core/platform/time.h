#ifndef TALABARTE_PLATFORM_TIME
#define TALABARTE_PLATFORM_TIME
#include "talabarte/types.h"

LocalTime platform_time_now(void);
u64 platform_time_millis(void);
u64 platform_time_nanos(void);
#endif