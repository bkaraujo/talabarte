#ifndef TALABARTE_CORE_APPLICATION
#define TALABARTE_CORE_APPLICATION
#include "talabarte/types.h"

API b8 application_initialize();
API b8 application_run(struct Game* game);
API void application_terminate();

#endif