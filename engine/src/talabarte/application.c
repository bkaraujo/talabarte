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
    if (!memory_initialize()) return 9;
    if (!platform_initialize()) return 9;
    if (!event_initialize()) return 9;
    if (!network_initialize()) return 9;
    if (!scripting_initialize()) return 9;
    if (!graphics_initialize()) return 9;
    if (!renderer_initialize()) return 9;
    if (!scene_initialize()) return 9;
    return TRUE;
}

b8 application_run() {
    INFO("application_run()");
    platform_window_show();

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