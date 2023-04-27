#include "talabarte/core/layer/lifecycle.h"
#include "talabarte/core/layer/buildin.h"
#include "talabarte/container/list.h"
#include "talabarte/layer.h"
#include "talabarte/memory.h"
#include "talabarte/logger.h"
#include "talabarte/string.h"

static Node* layers;
static struct Layer builtin;
b8 layer_initialize() {
    TTRACE("Initializing");
    layers = (Node*) memory_allocate(MEMORY_TAG_LIST_HEAD, sizeof(Node));

    layer_builtin_setup(&builtin);
    layer_attach(&builtin);

    return TRUE;
}

void layer_attach(struct Layer* layer) {
    if (layer == NULL) {
        TWARN("Ignoring NULL layer");
        return;
    }

    TTRACE("Attaching layer \"%s\"", layer->name);
    list_push(&layers, layer);
    layer->onAttach();
}

void layer_detach(struct Layer* layer) {
    if (layer == NULL) {
        TWARN("Ignoring NULL layer");
        return;
    }

    i32 found = -1;
    i32 index = -1;
    Node* current = layers;
    while (current->next != NULL) {
        index++;
        current = current->next;
        struct Layer* candidate = (struct Layer*) current;
        if (string_equals(candidate->name,layer->name)) found = index;
    }

    if (found != -1) {
        TTRACE("Detaching layer \"%s\"", layer->name);
        list_remove_by_index(&layers, found);
        layer->onDetach();
    }
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

void layer_terminate() {
    TTRACE("Terminating");
    layer_detach(&builtin);
    list_clear(&layers);
    memory_free(MEMORY_TAG_LIST_HEAD, layers, sizeof(Node));
    layers = NULL;
}
