#ifndef TALABARTE_MEMORY
#define TALABARTE_MEMORY
#include "talabarte/types.h"

b8 memory_initialize();
void memory_terminate();

enum MemoryTag {
    MEMORY_TAG_STRING,
    MEMORY_TAG_MAXIMUM
};

API void  memory_free(enum MemoryTag tag, void* block, u64 size);
API void* memory_allocate(enum MemoryTag tag, u64 size);
API void* memory_set( void* block, i32 value, u64 size);
API void  memory_zero(void* block, u64 size);
API void* memory_copy(void* source, void* target, u64 size);

#endif