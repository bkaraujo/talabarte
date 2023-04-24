#ifndef TALABARTE_EVENT
#define TALABARTE_EVENT
#include "talabarte/types.h"

typedef void (event_handler)(struct Event* event);
void event_fire(struct Event* event);
void event_register(enum EventCode code, event_handler* handler);

#endif