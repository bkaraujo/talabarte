#ifndef TALABARTE_LAYER
#define TALABARTE_LAYER
#include "talabarte/types.h"

struct Layer {
    void (*onAttach)(void);
    void (*onDetach)(void);
    void (*onUpdate)(f32 delta);
    void (*onGui)(f32 delta);
};

void layer_add(struct Layer* layer);

#endif