#include "talabarte/event/lifecycle.h"

#include "talabarte/logger.h"

b8 event_initialize() {
    DEBUG("event_initialize()");
    return TRUE;
}

void event_terminate() {
    DEBUG("event_terminate()");

}