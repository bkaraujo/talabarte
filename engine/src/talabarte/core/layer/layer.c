#include "talabarte/core/layer/lifecycle.h"
#include "talabarte/layer.h"
#include "talabarte/memory.h"
#include "talabarte/container/list.h"

void onAttach(void);
void onDetach(void);
void onUpdate(f32 delta);
void onGui(f32 delta);

static Node* layers;
static struct Layer layer;
b8 layer_initialize() {
    layers = (Node*) memory_allocate(MEMORY_TAG_LIST_HEAD, sizeof(Node));

    layer.onAttach = onAttach;
    layer.onDetach = onDetach;
    layer.onUpdate = onUpdate;
    layer.onGui = onGui;

    layer_add(&layer);

    return TRUE;
}

void layer_terminate() {
    list_clear(&layers);
    memory_free(MEMORY_TAG_LIST_HEAD, layers, sizeof(Node));
    layers = NULL;
}

void layer_onUpdate(f32 delta) {
    Node* current = layers;
    while (current->next != NULL) {
        current = current->next;
        ((struct Layer*) current->payload)->onUpdate(delta);
    }
}

void layer_onGui(f32 delta) {
    Node* current = layers;
    while (current->next != NULL) {
        current = current->next;
        ((struct Layer*) current->payload)->onGui(delta);
    }
}

void layer_add(struct Layer* layer) {
    layer->onAttach();
    list_push(&layers, layer);
}

void onAttach(void) {

}

void onDetach(void) {

}

void onUpdate(f32 delta) {
}

void onGui(f32 delta) {
}
