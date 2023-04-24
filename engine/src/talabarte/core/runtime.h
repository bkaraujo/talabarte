#ifndef TALABARTE_CORE_RUNTIME
#define TALABARTE_CORE_RUNTIME
#include "talabarte/types.h"

struct Runtime {
    b8 running;
    struct {
        struct {
            i32 width;
            i32 height;
        } Monitor;
        struct {
            const char* title;
            i32 width;
            i32 height;
        } Window;
        struct { } Keyboard;
        struct { } Mouse;
    } Platform;
    struct Game* Game;
};


struct Runtime* runtime_get();
#endif