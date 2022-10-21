#include "Engine/util.hpp"

void show_sdl_error(const char *label) {
    std::cout << "SDL ERROR: " << label << ": " << SDL_GetError() << std::endl;
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        show_sdl_error("SDL Initialization");
        return false;
    }

    int imgTypes = IMG_INIT_PNG;
    if ( !(IMG_Init(imgTypes) & imgTypes) ) {
        show_sdl_error("SDL_image Initialization");
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        show_sdl_error("SDL_mixer Initialization");
        return false;
    }

    return true;
}