#ifndef WINDOW_H
#define WINDOW_H
#pragma once

#include <SDL.h>
#include <utility>

#include "util.hpp"

class Window
{
public:
    Window(const char* title, int width, int height);
    ~Window();
    std::pair<int, int> getSize();
    bool isCreated() {return created;}
    SDL_Renderer* getRenderer() { return renderer; }

    void update() { SDL_RenderPresent(renderer); }
    void set_title(char* title) { SDL_SetWindowTitle(window, title); }

private:
    int w, h;
    bool created;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};


#endif