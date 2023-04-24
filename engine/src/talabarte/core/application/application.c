#include "talabarte/core/application/application.h"
#include "talabarte/core/application/eventhandler.h"

#include "talabarte/core/runtime.h"
#include "talabarte/core/platform/lifecycle.h"
#include "talabarte/core/platform/window.h"
#include "talabarte/core/event/lifecycle.h"
#include "talabarte/core/layer/lifecycle.h"
#include "talabarte/memory.h"
#include "talabarte/logger.h"
#include "talabarte/event.h"

static struct Runtime* runtime;
b8 application_initialize(struct Game* game) {
    runtime = runtime_get();
    runtime->running = TRUE;
    runtime->Game = game;
    
    if (!platform_initialize()) return FALSE;
    if (!memory_initialize()) return FALSE;
    if (!event_initialize()) return FALSE;
    if (!layer_initialize()) return FALSE;

    event_register(EVENT_APPLICATION_QUIT, application_event_quit);

    return TRUE;
}

b8 application_run() {
    INFO("application_run()");
    platform_window_show();
    
    while(runtime->running) {
        platform_onEvent();
        
        layer_onUpdate(0);
        layer_onGui(0);
    }

    platform_window_hide();
    return TRUE;
}

void application_terminate() {
    layer_terminate();
    event_terminate();
    platform_terminate();
    memory_terminate();
}