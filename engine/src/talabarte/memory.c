#include "talabarte/platform/memory.h"
#include "talabarte/memory.h"
#include "talabarte/logger.h"

struct MemoryRegistry {
    u64 total;
    u64 tag[MEMORY_TAG_MAXIMUM];
};

static struct MemoryRegistry registry;
b8 memory_initialize() {
    return TRUE;
}

void memory_terminate() {
    if (registry.total == 0) return;
    FATAL("Memory Leak: %0.2f MiB", MiB(KiB(registry.total)))
}

void memory_free(enum MemoryTag tag, void* block, u64 size) {
    registry.total -= size;
    registry.tag[tag] -= size;
    platform_memory_free(block);
}

void* memory_allocate(enum MemoryTag tag, u64 size) {
    registry.total += size;
    registry.tag[tag] += size;
    return platform_memory_allocate(size);
}

void* memory_set(void* block, i32 value, u64 size) {
    return platform_memory_set(block, value, size);
}

void memory_zero(void* block, u64 size) {
    platform_memory_set(block, 0, size);
}

void* memory_copy(void* source, void* target, u64 size) {
    return platform_memory_copy(source, target, size);
}