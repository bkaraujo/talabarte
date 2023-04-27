#ifndef TALABARTE_ENTRY
#define TALABARTE_ENTRY
#include "talabarte/core/application/application.h"
#include "talabarte/types.h"
#include "talabarte/logger.h"

b8 game_initialize(struct Game* game);
void game_terminate();

int main(void) {
    struct Game game;

    game.Window.title = "Talabarte Game";
    game.Window.maximized = FALSE;
    game.Window.resolution = RESOLUTION_HD;
    game.Window.aspectRatio = ASPECT_RATIO_16x9;

    if (!application_initialize()) { TFATAL("Application failed to initialize"); return 1; }
    if (!game_initialize(&game)) { TFATAL("GAME failed to initialize"); return 2; }
    if (!application_run(&game)) { TFATAL("Application failed to run"); return 3; }

    game_terminate();
    application_terminate();
    
    return 0;
}

#endif