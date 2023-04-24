#include "talabarte/core/application/eventhandler.h"

void application_event_quit(struct Event* event) {
    runtime_get()->running = FALSE;
    event->handled = TRUE;
}