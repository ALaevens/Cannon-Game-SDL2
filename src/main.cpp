#include "game.hpp"

#include <iostream>
#include <map>
#include <set>
#include <string>


int main(int argc, char **argv) {
    // log_out << "Path: " << argv[0] << "\n";

    if (!init()) {
        return -1;
    }

    Game* game = new Game();
    game->run();
    delete game;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}