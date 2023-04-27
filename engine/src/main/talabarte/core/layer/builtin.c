#include "talabarte/core/layer/buildin.h"

void builtin_layer_onAttach(void) {

}

void builtin_layer_onDetach(void) {

}

void builtin_layer_onUpdate(f32 delta) {
}

void builtin_layer_onGui(f32 delta) {

}

void layer_builtin_setup(struct Layer* layer) {
    layer->name = "Built-in";
    layer->onAttach = builtin_layer_onAttach;
    layer->onDetach = builtin_layer_onDetach;
    layer->onUpdate = builtin_layer_onUpdate;
    layer->onGui = builtin_layer_onGui;
}