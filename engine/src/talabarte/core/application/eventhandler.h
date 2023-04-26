#ifndef TALABARTE_CORE_APPLICATION_EVENT_HANDLER
#define TALABARTE_CORE_APPLICATION_EVENT_HANDLER
#include "talabarte/types.h"

void application_event_quit(struct Event* event);

void application_event_window_resized(struct Event* event);
void application_event_window_iconified(struct Event* event);
void application_event_window_maximized(struct Event* event);

#endif