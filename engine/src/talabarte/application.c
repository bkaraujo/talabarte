#include "talabarte/application.h"

#include "talabarte/platform/lifecycle.h"
#include "talabarte/platform/window.h"
#include "talabarte/event/lifecycle.h"
#include "talabarte/network/lifecycle.h"
#include "talabarte/scripting/lifecycle.h"
#include "talabarte/graphics/lifecycle.h"
#include "talabarte/renderer/lifecycle.h"
#include "talabarte/scene/lifecycle.h"
#include "talabarte/memory.h"
#include "talabarte/logger.h"

b8 application_initialize() {
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