#ifndef TALABARTE_CORE_PLATFORM_MEMORY
#define TALABARTE_CORE_PLATFORM_MEMORY
#include "talabarte/types.h"

void  platform_memory_free(void* block);
void* platform_memory_allocate(u64 size);
void* platform_memory_set(void* block, i32 value, u64 size);
void* platform_memory_copy(void* source, void* target, u64 size);

#endif