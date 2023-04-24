#ifndef TALABARTE_CONTAINER_LIST
#define TALABARTE_CONTAINER_LIST
#include "talabarte/types.h"

typedef struct Node {
    void* payload;
    struct Node* next;
} Node;

void list_push(Node** head, void* payload);
void* list_pop(Node** head);
void* list_remove_by_index(Node** head, int n);
void* list_remove_last(Node** head);
void list_clear(Node** head);

#endif