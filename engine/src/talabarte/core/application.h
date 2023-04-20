#ifndef TALABARTE_APPLICATION
#define TALABARTE_APPLICATION
#include "talabarte/types.h"

API b8 application_initialize(struct Game* game);
API b8 application_run();
API void application_terminate();

#endif