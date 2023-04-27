#ifndef TALABARTE_CORE_PLATFORM_INPUT
#define TALABARTE_CORE_PLATFORM_INPUT
#include "talabarte/types.h"

enum TALABARTE_KEY platform_input_key(u16 hid);
enum TALABARTE_BUTTON platform_input_button(u16 hid);

#endif