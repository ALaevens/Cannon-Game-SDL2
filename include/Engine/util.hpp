#ifndef SDL_UTIL_H
#define SDL_UTIL_H
#pragma once

#include <iostream>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "consts.hpp"

void show_sdl_error(const char *label);
bool init();

void log_out(std::stringstream& ss);

#endif //SDL_UTIL_H