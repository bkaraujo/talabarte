#include <talabarte/entry.h>

b8 game_initialize(struct Game* game) {
    game->Window.title = "Sandbox";
    game->Window.resolution = RESOLUTION_HD;

    return TRUE;
}

void game_terminate() {

}
