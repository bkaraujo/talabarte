#ifndef TALABARTE_CONTAINER_ARRAY
#define TALABARTE_CONTAINER_ARRAY
#include "talabarte/types.h"

void* array_create(u64 capacity, u64 stride);
void array_destroy(void* array);
u64 array_length(void* array);
u64 array_capacity(void* array);


void* _array_push_first(void* array, void* payload);
#define array_push_first(array, payload) { __typeof__(payload) temp = payload; array = _array_push_first(array, &temp); }

void* _array_push_last(void* array, void* payload);
#define array_push_last(array, payload) { __typeof__(payload) temp = payload; array = _array_push_last(array, &temp); }

void* _array_push(void* array, u64 index, void* payload);
#define array_push(array, index, payload) { __typeof__(payload) temp = payload; array = _array_push(array, index, &temp); }

void array_get_first(void* array, void* payload);
void array_get_last(void* array, void* payload);
void array_get(void* array, u64 index, void* payload);

void array_del_first(void* array);
void array_del_last(void* array);
void array_del(void* array, u64 index);
#endif