#include <talabarte/entry.h>

b8 game_configure(struct Game* game) {
    game->Window.title = "Sandbox";
    game->Window.resolution = RESOLUTION_HD;

    return TRUE;
}
