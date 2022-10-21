#include "game.hpp"

#include <iostream>
#include <map>
#include <set>
#include <string>

Mix_Chunk* load_sfx(const char *path) {
    Mix_Chunk* sfx = nullptr;
    sfx = Mix_LoadWAV(path);
    
    if (sfx == nullptr) {
        show_sdl_error("Sound effect could not be loaded");
    }

    return sfx;
}


int main(int argc, char **argv) {
    std::cout << "Path: " << argv[0] << std::endl;

    Game* game = new Game();
    game->run();
    delete game;

    IMG_Quit();
    SDL_Quit();

    return 0;
}