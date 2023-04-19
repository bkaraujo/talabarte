#ifndef TALABARTE_ENTRY
#define TALABARTE_ENTRY
#include "application.h"

int main(void) {
    if (!application_initialize()) return 9;
    if (!application_run()) return 9;
    application_terminate();
    return 0;
}

#endif