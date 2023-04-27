#include <talabarte/entry.h>
#include <talabarte/logger.h>

b8 game_initialize(struct Game* game) {
    TDEBUG("Initializing");
    game->Window.title = "Sandbox";
    game->Window.resolution = RESOLUTION_HD;

    return TRUE;
}

void game_terminate() {

}
