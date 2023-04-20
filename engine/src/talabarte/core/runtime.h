#ifndef TALABARTE_RUNTIME
#define TALABARTE_RUNTIME
#include "talabarte/types.h"

struct Runtime {
    struct {
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