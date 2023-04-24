#include "talabarte/container/list.h"
#include "talabarte/memory.h"

void list_push(Node** head, void* payload) {
    Node* current = *head;
    while (current->next != NULL) current = current->next;

    current->next = (Node*) memory_allocate(MEMORY_TAG_LIST_NODE, sizeof(Node));
    current->next->payload = payload;
    current->next->next = NULL;
}

void list_clear(Node** head) {
    if (*head == NULL) return;
    Node* next = (*head)->next;
    while (next != NULL) {
        Node* current = next;
        next = current->next;
        memory_free(MEMORY_TAG_LIST_NODE, current, sizeof(Node));
    }

}

void* list_pop(Node** head) {
    if (*head == NULL) { return NULL; }
    
    Node* next = (*head)->next;
    void* payload = (*head)->payload;

    memory_free(MEMORY_TAG_LIST_NODE, *head, sizeof(Node));
    *head = next;

    return payload;
}

void* list_remove_by_index(Node** head, int n) {
    if (n == 0) return list_pop(head);

    Node* current = *head;
    for (int i = 0; i < n - 1; i++) {
        if (current->next == NULL) return NULL;
        current = current->next;
    }

    if (current->next == NULL) return NULL;

    Node* temp = current->next;
    void* payload = temp->payload;
    current->next = temp->next;

    memory_free(MEMORY_TAG_LIST_NODE, temp, sizeof(Node));
    return payload;
}

void* list_remove_last(Node** head) {
    if ((*head)->next == NULL) return list_pop(head);

    Node* current = *head;
    while (current->next->next != NULL) current = current->next;

    void* payload = current->next->payload;
    memory_free(MEMORY_TAG_LIST_NODE, current->next, sizeof(Node));
    current->next = NULL;

    return payload;
}