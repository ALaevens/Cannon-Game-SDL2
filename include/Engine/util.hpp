#ifndef SDL_UTIL_H
#define SDL_UTIL_H
#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

void show_sdl_error(const char *label);
bool init();

#endif //SDL_UTIL_H