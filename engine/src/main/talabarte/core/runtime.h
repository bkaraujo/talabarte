#ifndef TALABARTE_CORE_RUNTIME
#define TALABARTE_CORE_RUNTIME
#include "talabarte/types.h"
#include "cglm/struct.h"

struct Runtime {
    // Indicates that the application is still running
    b8 running;

    struct {
        // Primary monitor information
        struct {
            i32 width;
            i32 height;
        } Monitor;
        
        // Window informations
        struct {
            const char* title;
            i32 width;
            i32 height;
            b8 iconified;
            b8 maximized;
        } Window;

        // Mouse current state
        struct {
            ivec2s position;
            b8 pressed[TALABARTE_BUTTON_MAXIMUM];
            i32 scroll;
        } Mouse;

        // Keyboard current state
        struct {
            b8 pressed[TALABARTE_KEY_MAXIMUM];
        } Keyboard;

    } Platform;
    struct Game* Game;
};


struct Runtime* runtime_get();
#endif