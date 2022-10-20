#include "Engine/util.hpp"

void show_sdl_error(const char *label) {
    std::cout << "SDL ERROR: " << label << ": " << SDL_GetError() << std::endl;
}