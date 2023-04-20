#ifndef TALABARTE_TYPES
#define TALABARTE_TYPES

#include <stddef.h>

#ifdef EXPORT
    #if _MSC_VER
        #define API __declspec(dllexport)
    #else
        #define API __attribute__((visibility("default")))
    #endif
#else
    #if _MSC_VER
        #define API __declspec(dllimport)
    #else
        #define API
    #endif
#endif

// Unsigned int types.
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;
// Signed int types.
typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;
// Floating point types
typedef float               f32;
typedef double              f64;
// Boolean types
typedef int                 b32;
typedef _Bool               b8;

#define TRUE 1
#define FALSE 0

typedef struct {
    u16 year;
    u16 month;
    u16 day;

    u16 hour;
    u16 minute;
    u16 second;
    u16 millis;
} LocalTime;

#define KiB(bytes) (f32) (bytes / 1024.0)
#define MiB(kib) (f32) (kib / 1024.0)
#define GiB(mib) (f32) (mib / 1024.0)

enum AspectRatio { ASPECT_RATIO_4x3, ASPECT_RATIO_16x9, ASPECT_RATIO_21x9 };
enum Resolution { RESOLUTION_HD, RESOLUTION_FHD, RESOLUTION_QHD, RESOLUTION_UHD };

struct Game {
    struct {
        const char* title;
        enum Resolution resolution;
        enum AspectRatio aspectRatio;
        b8 maximized;
    } Window;
};

#endif