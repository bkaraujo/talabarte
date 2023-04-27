#include "talabarte/core/event/lifecycle.h"
#include "talabarte/container/list.h"
#include "talabarte/event.h"
#include "talabarte/memory.h"
#include "talabarte/logger.h"

static Node* registry[EVENT_MAXIMUM];
b8 event_initialize() {
    TTRACE("Initializing");
    for (int i = 0 ; i < EVENT_MAXIMUM; ++i)
        registry[i] = (Node*) memory_allocate(MEMORY_TAG_LIST_HEAD, sizeof(Node));

    return TRUE;
}

void event_register(enum EventCode code, EventHandler* handler) {
    list_push(&registry[code], handler);
}

void event_fire(struct Event* event) {
    if (event->handled) return;
    
    Node* current = registry[event->code];
    while (current->next != NULL) {
        current = current->next;
        ((EventHandler*) current->payload)(event);
        if (event->handled) break;
    }
}

void event_terminate() {
    TTRACE("Terminating");
    for (int i = 0 ; i < EVENT_MAXIMUM; ++i) {
        list_clear(&registry[i]);
        memory_free(MEMORY_TAG_LIST_HEAD, registry[i], sizeof(Node));
    }
}