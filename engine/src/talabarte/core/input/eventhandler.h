#ifndef TALABARTE_CORE_INPUT_EVENT_HANDLER
#define TALABARTE_CORE_INPUT_EVENT_HANDLER
#include "talabarte/types.h"

void input_event_key_pressed(struct Event* event);
void input_event_key_released(struct Event* event);

void input_event_mouse_pressed(struct Event* event);
void input_event_mouse_released(struct Event* event);
void input_event_mouse_moved(struct Event* event);
void input_event_mouse_scrolled(struct Event* event);


#endif