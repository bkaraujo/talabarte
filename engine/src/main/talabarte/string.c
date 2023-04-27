#include "talabarte/string.h"
#include "talabarte/memory.h"

#include "talabarte/core/platform/detector.h"
#include <string.h>

b8 string_equals(const char* str0, const char* str1) {
    return strcmp(str0, str1) == 0;
}

b8 string_iequals(const char* str0, const char* str1) {
    #ifdef TALABARTE_PLATFORM_WINDOWS
        return _stricmp(str0, str1) == 0;
    #else
        return strcasecmp(str0, str1) == 0;
    #endif
}

u32 string_length(const char* str) {
    return strlen(str);
}