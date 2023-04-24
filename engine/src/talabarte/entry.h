#ifndef TALABARTE_ENTRY
#define TALABARTE_ENTRY
#include "talabarte/core/application/application.h"
#include "talabarte/types.h"

b8 game_initialize(struct Game* game);
void game_terminate();

int main(void) {
    struct Game game;

    game.Window.title = "Talabarte Game";
    game.Window.maximized = FALSE;
    game.Window.resolution = RESOLUTION_HD;
    game.Window.aspectRatio = ASPECT_RATIO_16x9;

    if (!game_initialize(&game)) return 1;
    if (!application_initialize(&game)) return 2;
    if (!application_run()) return 9;
    application_terminate();
    game_terminate();
    
    return 0;
}

#endif