#include "talabarte/core/application/eventhandler.h"
#include "talabarte/core/runtime.h"

void application_event_quit(struct Event* event) {
    runtime_get()->running = FALSE;
    event->handled = TRUE;
}

void application_event_window_resized(struct Event* event) {
    struct Runtime* runtime = runtime_get();
    runtime->Platform.Window.width = event->payload.u32[0];
    runtime->Platform.Window.height = event->payload.u32[1];
}

void application_event_window_iconified(struct Event* event) {
    struct Runtime* runtime = runtime_get();

    runtime->Platform.Window.iconified = TRUE;
    runtime->Platform.Window.maximized = FALSE;
    runtime->Platform.Window.width = event->payload.u32[0];
    runtime->Platform.Window.height = event->payload.u32[1];
}

void application_event_window_maximized(struct Event* event) {
    struct Runtime* runtime = runtime_get();

    runtime->Platform.Window.maximized = TRUE;
    runtime->Platform.Window.iconified = FALSE;
    runtime->Platform.Window.width = event->payload.u32[0];
    runtime->Platform.Window.height = event->payload.u32[1];
}