#include "talabarte/container/array.h"
#include "talabarte/memory.h"
#include "talabarte/logger.h"

enum {
    FIELD_CAPACITY,
    FIELD_LENGTH,
    FIELD_STRIDE,
    FIELD_MAXIMUM
};

u64 array_field(void* array, u64 field) {
    if (array == NULL) { TFATAL("NULL array"); return -1; }
     u64* header = (u64*)array - FIELD_MAXIMUM;
    return header[field];
}

void array_field_set(void* array, u64 field, u64 value) {
    if (array == NULL) { TFATAL("NULL array"); return; }
     u64* header = (u64*)array - FIELD_MAXIMUM;
    header[field] = value;
}

u64 array_capacity(void* array) { return array_field(array, FIELD_CAPACITY); }
u64 array_length  (void* array) { return array_field(array, FIELD_LENGTH); }
u64 array_stride  (void* array) { return array_field(array, FIELD_STRIDE); }

// capacity - length - stride - payloar[n]
void* array_create(u64 capacity, u64 stride) {
    u64 hcapacity = sizeof(u64) * FIELD_MAXIMUM;
    u64 acapacity = stride * capacity;
    void* array = memory_allocate(MEMORY_TAG_ARRAY, hcapacity + acapacity);
    memory_zero(array, hcapacity + acapacity);

    array_field_set(array, FIELD_CAPACITY, capacity);
    array_field_set(array, FIELD_LENGTH, 0);
    array_field_set(array, FIELD_STRIDE, stride);

    return (void*)(array + FIELD_MAXIMUM);
}

void array_destroy(void* array) {
    if (array == NULL) {
        TFATAL("Trying to destroy a NULL array");
        return;
    }

    u64 hcapacity = sizeof(u64) * FIELD_MAXIMUM;
    u64 acapacity = array_capacity(array) * array_stride(array);
    memory_free(MEMORY_TAG_ARRAY, array, hcapacity + acapacity);
}

void* array_resize(void* array) {
    void* created = array_create((array_capacity(array) * 1.75) + 1, array_stride(array));
    memory_copy(array, created, array_length(array));
    array_destroy(array);
    
    return created;
}

void* _array_push_first(void* array, void* payload) {
    return _array_push(array, 0, payload);
}

void* _array_push_last(void* array, void* payload) {
    return _array_push(array, array_length(array), payload);
}

void* _array_push(void* array, u64 index, void* payload) {
    if (array == NULL) {
        TFATAL("NULL array");
        return NULL;
    }

    u64 capacity = array_capacity(array);
    u64 length = array_length(array);

    if (index > capacity) {
        TFATAL("Index (%u) out of bounds (%u).", index, capacity);
        return array;
    }
    
    if (index > length) {
        TFATAL("Index (%u) beyond the end (%u). Consider using array_push_last(...)", index, length);
        return array;
    }

    if (length == capacity) array = array_resize(array);
    u64* address = (u64*) array;

    // move the items forward so we can insert in the middel of the array.
    u64 stride = array_stride(array);
    if (index < length) {
        memory_copy(
            address + (index * stride), 
            address + ((index + 1) * stride), 
            stride * (length - index)
        );
    }

    memory_copy(payload, address + (index * stride), stride);
    
    array_field_set(array, FIELD_LENGTH, array_length(array) + 1);
    return array;
}

void array_get_first(void* array, void* payload) {
    array_get(array, 0, payload);
}

void array_get_last(void* array, void* payload) {
    array_get(array, array_length(array), payload);
}

void array_get(void* array, u64 index, void* payload) {
    if (array == NULL) {
        TFATAL("NULL array");
        payload = NULL;
        return;
    }

    u64 capacity = array_capacity(array);
    u64 length = array_length(array);
    u64 stride = array_stride(array);

    if (index > capacity) {
        TFATAL("Index (%u) out of bounds (%u).", index, capacity);
        payload = NULL;
        return;
    }
    
    if (index > length) {
        TFATAL("Index (%u) beyond the end (%u). Consider using array_get_last(...)", index, length);
        payload = NULL;
        return;
    }
    
    u64* address = (u64*) array;
    memory_copy(address + (index * stride), payload, stride);
}


void array_del_first(void* array) {
    array_del(array, 0);
}

void array_del_last(void* array) {
    array_del(array, array_length(array));
}

void array_del(void* array, u64 index) {
    if (array == NULL) {
        TFATAL("NULL array");
        return;
    }

    u64 capacity = array_capacity(array);
    u64 length = array_length(array);
    u64 stride = array_stride(array);

    if (index > capacity) {
        TFATAL("Index (%u) out of bounds (%u).", index, capacity);
        return;
    }
    
    if (index > length) {
        TFATAL("Index (%u) beyond the end (%u). Consider using array_del_last(...)", index, length);
        return;
    }
    
    u64* address = (u64*) array;
    memory_copy(
        address + ((index + 1)* stride), 
        address + ((index) * stride), 
        stride * (length - index - 1)
    );

    array_field_set(array, FIELD_LENGTH, array_length(array) - 1);
}
