#include "talabarte/core/runtime.h"

static struct Runtime runtime;
struct Runtime* runtime_get() {
    return &runtime;
}