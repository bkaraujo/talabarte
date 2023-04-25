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

    EVENT_MOUSE_PRESSED,
    EVENT_MOUSE_RELEASED,
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

enum TALABARTE_KEY {

    TALABARTE_KEY_ESCAPE,
    TALABARTE_KEY_F1,
    TALABARTE_KEY_F2,
    TALABARTE_KEY_F3,
    TALABARTE_KEY_F4,
    TALABARTE_KEY_F5,
    TALABARTE_KEY_F6,
    TALABARTE_KEY_F7,
    TALABARTE_KEY_F8,
    TALABARTE_KEY_F9,
    TALABARTE_KEY_F10,
    TALABARTE_KEY_F11,
    TALABARTE_KEY_F12,
    TALABARTE_KEY_F13,
    TALABARTE_KEY_F14,
    TALABARTE_KEY_F15,
    TALABARTE_KEY_F16,
    TALABARTE_KEY_F17,
    TALABARTE_KEY_F18,
    TALABARTE_KEY_F19,
    TALABARTE_KEY_F20,
    TALABARTE_KEY_F21,
    TALABARTE_KEY_F22,
    TALABARTE_KEY_F23,
    TALABARTE_KEY_F24,
    TALABARTE_KEY_PRINT,
    TALABARTE_KEY_SCROLL_LOCK,
    TALABARTE_KEY_PAUSE,

    TALABARTE_KEY_OEM_3,
    TALABARTE_KEY_0,
    TALABARTE_KEY_1,
    TALABARTE_KEY_2,
    TALABARTE_KEY_3,
    TALABARTE_KEY_4,
    TALABARTE_KEY_5,
    TALABARTE_KEY_6,
    TALABARTE_KEY_7,
    TALABARTE_KEY_8,
    TALABARTE_KEY_9,
    TALABARTE_KEY_OEM_MINUS,
    TALABARTE_KEY_OEM_PLUS,
    TALABARTE_KEY_OEM_BACKSPACE,
    TALABARTE_KEY_INSERT,
    TALABARTE_KEY_HOME,
    TALABARTE_KEY_PAGEUP,

    TALABARTE_KEY_TAB,
    TALABARTE_KEY_Q,
    TALABARTE_KEY_W,
    TALABARTE_KEY_E,
    TALABARTE_KEY_R,
    TALABARTE_KEY_T,
    TALABARTE_KEY_Y,
    TALABARTE_KEY_U,
    TALABARTE_KEY_I,
    TALABARTE_KEY_O,
    TALABARTE_KEY_P,
    TALABARTE_KEY_OEM_4,
    TALABARTE_KEY_OEM_5,
    TALABARTE_KEY_RETURN,
    TALABARTE_KEY_DELETE,
    TALABARTE_KEY_END,
    TALABARTE_KEY_PAGEDOWN,

    TALABARTE_KEY_A,
    TALABARTE_KEY_B,
    TALABARTE_KEY_C,
    TALABARTE_KEY_D,
    TALABARTE_KEY_F,
    TALABARTE_KEY_G,
    TALABARTE_KEY_H,
    TALABARTE_KEY_J,
    TALABARTE_KEY_K,
    TALABARTE_KEY_L,
    TALABARTE_KEY_M,
    TALABARTE_KEY_N,
    TALABARTE_KEY_S,
    TALABARTE_KEY_V,
    TALABARTE_KEY_X,
    TALABARTE_KEY_Z,

    TALABARTE_KEY_BANG,
    TALABARTE_KEY_AT,
    TALABARTE_KEY_HASH,
    TALABARTE_KEY_DOLAR,
    TALABARTE_KEY_PERCENT,
    TALABARTE_KEY_CARET,
    TALABARTE_KEY_AMPERSAND,
    TALABARTE_KEY_START,
    TALABARTE_KEY_LEFT_BRACKET,
    TALABARTE_KEY_RIGHT_BRACKET,

    TALABARTE_KEY_MAXIMUM
};

enum TALABARTE_BUTTON {
    TALABARTE_BUTTON_1,
    TALABARTE_BUTTON_2,
    TALABARTE_BUTTON_3,
    TALABARTE_BUTTON_MAXIMUM
};
#endif