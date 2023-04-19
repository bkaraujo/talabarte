#include "talabarte/scripting/lifecycle.h"

#include "talabarte/logger.h"

b8 scripting_initialize() {
    DEBUG("scripting_initialize()");
    return TRUE;
}

void scripting_terminate() {
    DEBUG("scripting_terminate()");

}