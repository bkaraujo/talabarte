#include "talabarte/core/application/eventhandler.h"
#include "talabarte/core/runtime.h"

void application_event_quit(struct Event* event) {
    runtime_get()->running = FALSE;
    event->handled = TRUE;
}