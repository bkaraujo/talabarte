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
    TALABARTE_KEY_BACKSPACE = 0x08,
    TALABARTE_KEY_ENTER = 0x0D,
    TALABARTE_KEY_TAB = 0x09,
    TALABARTE_KEY_SHIFT = 0x10,
    TALABARTE_KEY_CONTROL = 0x11,

    TALABARTE_KEY_PAUSE = 0x13,
    TALABARTE_KEY_CAPITAL = 0x14,

    TALABARTE_KEY_ESCAPE = 0x1B,

    TALABARTE_KEY_CONVERT = 0x1C,
    TALABARTE_KEY_NONCONVERT = 0x1D,
    TALABARTE_KEY_ACCEPT = 0x1E,
    TALABARTE_KEY_MODECHANGE = 0x1F,

    TALABARTE_KEY_SPACE = 0x20,
    TALABARTE_KEY_PAGEUP = 0x21,
    TALABARTE_KEY_PAGEDOWN = 0x22,
    TALABARTE_KEY_END = 0x23,
    TALABARTE_KEY_HOME = 0x24,
    TALABARTE_KEY_LEFT = 0x25,
    TALABARTE_KEY_UP = 0x26,
    TALABARTE_KEY_RIGHT = 0x27,
    TALABARTE_KEY_DOWN = 0x28,
    TALABARTE_KEY_SELECT = 0x29,
    TALABARTE_KEY_PRINT = 0x2A,
    TALABARTE_KEY_EXECUTE = 0x2B,
    TALABARTE_KEY_PRINTSCREEN = 0x2C,
    TALABARTE_KEY_INSERT = 0x2D,
    TALABARTE_KEY_DELETE = 0x2E,
    TALABARTE_KEY_HELP = 0x2F,

    TALABARTE_KEY_0 = 0x30,
    TALABARTE_KEY_1 = 0x31,
    TALABARTE_KEY_2 = 0x32,
    TALABARTE_KEY_3 = 0x33,
    TALABARTE_KEY_4 = 0x34,
    TALABARTE_KEY_5 = 0x35,
    TALABARTE_KEY_6 = 0x36,
    TALABARTE_KEY_7 = 0x37,
    TALABARTE_KEY_8 = 0x38,
    TALABARTE_KEY_9 = 0x39,

    TALABARTE_KEY_A = 0x41,
    TALABARTE_KEY_B = 0x42,
    TALABARTE_KEY_C = 0x43,
    TALABARTE_KEY_D = 0x44,
    TALABARTE_KEY_E = 0x45,
    TALABARTE_KEY_F = 0x46,
    TALABARTE_KEY_G = 0x47,
    TALABARTE_KEY_H = 0x48,
    TALABARTE_KEY_I = 0x49,
    TALABARTE_KEY_J = 0x4A,
    TALABARTE_KEY_K = 0x4B,
    TALABARTE_KEY_L = 0x4C,
    TALABARTE_KEY_M = 0x4D,
    TALABARTE_KEY_N = 0x4E,
    TALABARTE_KEY_O = 0x4F,
    TALABARTE_KEY_P = 0x50,
    TALABARTE_KEY_Q = 0x51,
    TALABARTE_KEY_R = 0x52,
    TALABARTE_KEY_S = 0x53,
    TALABARTE_KEY_T = 0x54,
    TALABARTE_KEY_U = 0x55,
    TALABARTE_KEY_V = 0x56,
    TALABARTE_KEY_W = 0x57,
    TALABARTE_KEY_X = 0x58,
    TALABARTE_KEY_Y = 0x59,
    TALABARTE_KEY_Z = 0x5A,

    TALABARTE_KEY_SUPER_L = 0x5B,
    TALABARTE_KEY_SUPER_R = 0x5C,
    TALABARTE_KEY_APPS = 0x5D,

    TALABARTE_KEY_SLEEP = 0x5F,

    TALABARTE_KEY_NUMPAD0 = 0x60,
    TALABARTE_KEY_NUMPAD1 = 0x61,
    TALABARTE_KEY_NUMPAD2 = 0x62,
    TALABARTE_KEY_NUMPAD3 = 0x63,
    TALABARTE_KEY_NUMPAD4 = 0x64,
    TALABARTE_KEY_NUMPAD5 = 0x65,
    TALABARTE_KEY_NUMPAD6 = 0x66,
    TALABARTE_KEY_NUMPAD7 = 0x67,
    TALABARTE_KEY_NUMPAD8 = 0x68,
    TALABARTE_KEY_NUMPAD9 = 0x69,
    TALABARTE_KEY_MULTIPLY = 0x6A,
    TALABARTE_KEY_ADD = 0x6B,
    TALABARTE_KEY_SEPARATOR = 0x6C,
    TALABARTE_KEY_SUBTRACT = 0x6D,
    TALABARTE_KEY_DECIMAL = 0x6E,
    TALABARTE_KEY_DIVIDE = 0x6F,

    TALABARTE_KEY_F1 = 0x70,
    TALABARTE_KEY_F2 = 0x71,
    TALABARTE_KEY_F3 = 0x72,
    TALABARTE_KEY_F4 = 0x73,
    TALABARTE_KEY_F5 = 0x74,
    TALABARTE_KEY_F6 = 0x75,
    TALABARTE_KEY_F7 = 0x76,
    TALABARTE_KEY_F8 = 0x77,
    TALABARTE_KEY_F9 = 0x78,
    TALABARTE_KEY_F10 = 0x79,
    TALABARTE_KEY_F11 = 0x7A,
    TALABARTE_KEY_F12 = 0x7B,
    TALABARTE_KEY_F13 = 0x7C,
    TALABARTE_KEY_F14 = 0x7D,
    TALABARTE_KEY_F15 = 0x7E,
    TALABARTE_KEY_F16 = 0x7F,
    TALABARTE_KEY_F17 = 0x80,
    TALABARTE_KEY_F18 = 0x81,
    TALABARTE_KEY_F19 = 0x82,
    TALABARTE_KEY_F20 = 0x83,
    TALABARTE_KEY_F21 = 0x84,
    TALABARTE_KEY_F22 = 0x85,
    TALABARTE_KEY_F23 = 0x86,
    TALABARTE_KEY_F24 = 0x87,

    TALABARTE_KEY_NUMLOCK = 0x90,
    TALABARTE_KEY_SCROLL = 0x91,
    TALABARTE_KEY_NUMPAD_EQUAL = 0x92,

    TALABARTE_KEY_SHIFT_L = 0xA0,
    TALABARTE_KEY_SHIFT_R = 0xA1,
    TALABARTE_KEY_CONTROL_L = 0xA2,
    TALABARTE_KEY_CONTROL_R = 0xA3,
    TALABARTE_KEY_ALT_L = 0xA4,
    TALABARTE_KEY_ALT_R = 0xA5,

    TALABARTE_KEY_SEMICOLON = 0x3B,

    TALABARTE_KEY_APOSTROPHE = 0xDE,
    TALABARTE_KEY_QUOTE = TALABARTE_KEY_APOSTROPHE,
    TALABARTE_KEY_EQUAL = 0xBB,
    TALABARTE_KEY_COMMA = 0xBC,
    TALABARTE_KEY_MINUS = 0xBD,
    TALABARTE_KEY_PERIOD = 0xBE,
    TALABARTE_KEY_SLASH = 0xBF,

    TALABARTE_KEY_GRAVE = 0xC0,

    TALABARTE_KEY_PIPE = 0xDC,
    TALABARTE_KEY_BACKSLASH = TALABARTE_KEY_PIPE,
    TALABARTE_KEY_BRACKET_L = 0xDB,
    TALABARTE_KEY_BRACKET_R = 0xDD,
    TALABARTE_KEY_MAXIMUM = 0xFF
};

enum TALABARTE_BUTTON {
    TALABARTE_BUTTON_1,
    TALABARTE_BUTTON_2,
    TALABARTE_BUTTON_3,
    TALABARTE_BUTTON_MAXIMUM
};

struct Layer {
    const char* name;
    void (*onAttach)(void);
    void (*onDetach)(void);
    void (*onUpdate)(f32 delta);
    void (*onGui)(f32 delta);
};

#endif