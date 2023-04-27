#ifndef TALABARTE_EVENT
#define TALABARTE_EVENT
#include "talabarte/types.h"

typedef void (EventHandler)(struct Event* event);
void event_fire(struct Event* event);
void event_register(enum EventCode code, EventHandler* handler);

#endif