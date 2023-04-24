#ifndef TALABARTE_CORE_LAYER
#define TALABARTE_CORE_LAYER
#include "talabarte/types.h"

b8 layer_initialize();
void layer_terminate();

void layer_onUpdate(f32 delta);
void layer_onGui(f32 delta);

#endif