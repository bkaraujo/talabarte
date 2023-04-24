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

enum EventCode {
    EVENT_APPLICATION_QUIT,

    EVENT_WINDOW_ICONIFIED,
    EVENT_WINDOW_MAXIMIZED,
    EVENT_WINDOW_RESIZED,

    EVENT_KEY_PRESSED,
    EVENT_KEY_RELEASED,

    EVENT_MOUSE_BUTTON_PRESSED,
    EVENT_MOUSE_BUTTON_RELEASED,
    EVENT_MOUSE_MOVED,
    EVENT_MOUSE_SCROLLED,

    EVENT_MAXIMUM  
};

struct Event {
    enum EventCode code;
    b8 handled;
    union {
        i64 i64[2];
        u64 u64[2];
        f64 f64[2];

        i32 i32[4];
        u32 u32[4];
        f32 f32[4];

        i16 i16[8];
        u16 u16[8];

        i8 i8[16];
        u8 u8[16];
        char c[16];
    } payload;
};
#endif