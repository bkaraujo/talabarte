#ifndef TALABARTE_CORE_GRAPHICS
#define TALABARTE_CORE_GRAPHICS
#include "talabarte/types.h"

b8 graphics_initialize();
void graphics_terminate();

void graphics_compute_size(enum AspectRatio ratio, enum Resolution resolution, i32* width, i32* height);

#endif