#include "talabarte/core/input/lifecycle.h"
#include "talabarte/core/input/eventhandler.h"
#include "talabarte/input.h"
#include "talabarte/memory.h"
#include "talabarte/event.h"
#include "talabarte/core/runtime.h"

#include "cglm/cglm.h"

struct Keyboard {
    b8 key[TALABARTE_KEY_MAXIMUM];
};

struct Mouse {
    b8 button[TALABARTE_BUTTON_MAXIMUM];
    ivec2s position;
};

struct Context {
    struct {
        struct Keyboard keyboard;
        struct Mouse mouse;
    } current;
    struct {
        struct Keyboard keyboard;
        struct Mouse mouse;
    } previous;
};

static struct Context context;

b8 input_initialize() {
    memory_zero(&context.current, sizeof(struct Keyboard) + sizeof(struct Mouse));
    memory_zero(&context.current, sizeof(struct Keyboard) + sizeof(struct Mouse));

    event_register(EVENT_KEY_PRESSED, input_event_key_pressed);
    event_register(EVENT_KEY_RELEASED, input_event_key_released);
    event_register(EVENT_MOUSE_PRESSED, input_event_mouse_pressed);
    event_register(EVENT_MOUSE_RELEASED, input_event_mouse_released);
    event_register(EVENT_MOUSE_MOVED, input_event_mouse_moved);
    event_register(EVENT_MOUSE_SCROLLED, input_event_mouse_scrolled);

    return TRUE;
}

void input_update() {
    memory_copy(&context.current, &context.previous, sizeof(struct Keyboard) + sizeof(struct Mouse));
    memory_zero(&context.current, sizeof(struct Keyboard) + sizeof(struct Mouse));
    
    struct Runtime* runtime = runtime_get();
    memory_copy(runtime->Platform.Keyboard.pressed, &context.current.keyboard.key, sizeof(context.current.keyboard.key));
    memory_copy(&runtime->Platform.Mouse.position, &context.current.mouse.position, sizeof(ivec2s));
    memory_copy(runtime->Platform.Mouse.pressed, &context.current.mouse.button, sizeof(context.current.mouse.button));
}

void input_terminate() {
    memory_zero(&context.previous, sizeof(struct Keyboard) + sizeof(struct Mouse));
    memory_zero(&context.current, sizeof(struct Keyboard) + sizeof(struct Mouse));
}

b8 input_key_active(enum TALABARTE_KEY key) {
    return context.current.keyboard.key[key];
}

b8 input_key_pressed(enum TALABARTE_KEY key) {
    return !context.previous.keyboard.key[key] && context.current.keyboard.key[key];
}

b8 input_key_releaded(enum TALABARTE_KEY key) {
    return context.previous.keyboard.key[key] && !context.current.keyboard.key[key];
}

b8 input_mouse_pressed(enum TALABARTE_BUTTON button) {
    return !context.previous.mouse.button[button] && context.current.mouse.button[button];
}

b8 input_mouse_releaded(enum TALABARTE_BUTTON button) {
    return context.previous.mouse.button[button] && !context.current.mouse.button[button];;
}

ivec2s input_get_mouse_position() {
    ivec2s copy;
    copy.x = context.current.mouse.position.x;
    copy.y = context.current.mouse.position.y;

    return copy;
}

void input_get_mouse_scroll() {

}