#include "talabarte/core/application/eventhandler.h"

b8 application_event_quit(struct Event* event) {
    runtime_get()->running = FALSE;
    return TRUE;
}