#ifndef TALABARTE_INPUT
#define TALABARTE_INPUT
#include "talabarte/types.h"
#include "cglm/struct.h"

API b8 input_key_active(enum TALABARTE_KEY key);
API b8 input_key_pressed(enum TALABARTE_KEY key);
API b8 input_key_releaded(enum TALABARTE_KEY key);

API b8 input_mouse_pressed(enum TALABARTE_BUTTON button);
API b8 input_mouse_releaded(enum TALABARTE_BUTTON button);

API ivec2s input_get_mouse_position();
API void input_get_mouse_scroll();

#endif