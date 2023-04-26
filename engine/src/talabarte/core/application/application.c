#include "talabarte/core/application/application.h"
#include "talabarte/core/application/eventhandler.h"

#include "talabarte/core/runtime.h"
#include "talabarte/core/input/lifecycle.h"
#include "talabarte/core/platform/lifecycle.h"
#include "talabarte/core/platform/window.h"
#include "talabarte/core/event/lifecycle.h"
#include "talabarte/core/layer/lifecycle.h"
#include "talabarte/memory.h"
#include "talabarte/event.h"
#include "talabarte/input.h"

static struct Runtime* runtime;
b8 application_initialize(struct Game* game) {
    runtime = runtime_get();
    runtime->running = TRUE;
    runtime->Game = game;
    
    if (!platform_initialize()) return FALSE;
    if (!memory_initialize()) return FALSE;
    if (!event_initialize()) return FALSE;
    if (!input_initialize()) return FALSE;
    if (!layer_initialize()) return FALSE;

    event_register(EVENT_APPLICATION_QUIT, application_event_quit);
    
    event_register(EVENT_WINDOW_RESIZED, application_event_window_resized);
    event_register(EVENT_WINDOW_ICONIFIED, application_event_window_iconified);
    event_register(EVENT_WINDOW_MAXIMIZED, application_event_window_maximized);

    return TRUE;
}

void application_update();

b8 application_run() {
    platform_window_show();
    
    while(runtime->running) {
        if (runtime->Platform.Window.iconified) {
            platform_update();
            continue;
        }
        
        if (input_key_pressed(TALABARTE_KEY_ESCAPE)) runtime->running = FALSE;

        layer_onUpdate(0);
        layer_onGui(0);

        platform_update();
        input_update();
    }

    platform_window_hide();
    return TRUE;
}

void application_terminate() {
    layer_terminate();
    input_terminate();
    event_terminate();
    platform_terminate();
    memory_terminate();
}