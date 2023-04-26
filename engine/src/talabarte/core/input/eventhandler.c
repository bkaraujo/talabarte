#include "talabarte/core/input/eventhandler.h"
#include "talabarte/core/runtime.h"

void input_event_key_pressed(struct Event* event) {
    struct Runtime* runtime = runtime_get();
    runtime->Platform.Keyboard.pressed[event->payload.u16[0]] = TRUE;
    event->handled = TRUE;
}

void input_event_key_released(struct Event* event) {
    struct Runtime* runtime = runtime_get();
    runtime->Platform.Keyboard.pressed[event->payload.u16[0]] = FALSE;
    event->handled = TRUE;
}

void input_event_mouse_pressed(struct Event* event) {
    struct Runtime* runtime = runtime_get();
    runtime->Platform.Mouse.pressed[event->payload.u8[0]] = TRUE;
    event->handled = TRUE;
}

void input_event_mouse_released(struct Event* event) {
    struct Runtime* runtime = runtime_get();
    runtime->Platform.Mouse.pressed[event->payload.u8[0]] = FALSE;
    event->handled = TRUE;
}

void input_event_mouse_moved(struct Event* event) {
    struct Runtime* runtime = runtime_get();
    runtime->Platform.Mouse.position.x = event->payload.i32[0];
    runtime->Platform.Mouse.position.y = event->payload.i32[1];
    event->handled = TRUE;
}

void input_event_mouse_scrolled(struct Event* event) {
    struct Runtime* runtime = runtime_get();
    runtime->Platform.Mouse.scroll += event->payload.i32[0];
    event->handled = TRUE;
}
