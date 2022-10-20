#include "Engine/Window.hpp"

Window::Window(const char *title, int w, int h) {
    this->w = w;
    this->h = h;
    created = false;

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->w, this->h, SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        show_sdl_error("Window not created");
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        show_sdl_error("Renderer not created");
        return;
    }

    created = true;
}

Window::~Window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}