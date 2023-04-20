#include "talabarte/core/application.h"

#include "talabarte/core/runtime.h"
#include "talabarte/core/platform/lifecycle.h"
#include "talabarte/core/platform/window.h"
#include "talabarte/core/event/lifecycle.h"
#include "talabarte/core/network/lifecycle.h"
#include "talabarte/core/scripting/lifecycle.h"
#include "talabarte/core/graphics/lifecycle.h"
#include "talabarte/core/renderer/lifecycle.h"
#include "talabarte/core/scene/lifecycle.h"
#include "talabarte/memory.h"
#include "talabarte/logger.h"

b8 application_initialize(struct Game* game) {
    runtime_get()->Game = game;
    
    if (!memory_initialize()) return FALSE;
    if (!platform_initialize()) return FALSE;
    if (!event_initialize()) return FALSE;
    if (!network_initialize()) return FALSE;
    if (!scripting_initialize()) return FALSE;
    if (!graphics_initialize()) return FALSE;
    if (!renderer_initialize()) return FALSE;
    if (!scene_initialize()) return FALSE;

    return TRUE;
}

b8 application_run() {
    INFO("application_run()");
    platform_window_show();
    
    while(TRUE) {
        platform_onEvent();
    }

    platform_window_hide();
    return TRUE;
}

void application_terminate() {
    scene_terminate();
    renderer_terminate();
    graphics_terminate();
    scripting_terminate();
    network_terminate();
    event_terminate();
    platform_terminate();
    memory_terminate();
}