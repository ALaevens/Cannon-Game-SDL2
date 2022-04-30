#include "Window.hpp"
#include "util.hpp"
#include "Clock.hpp"
#include "TextureManager.hpp"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <map>
#include <set>
#include <string>

const int SCREEN_W = 1280;
const int SCREEN_H = 720;

Mix_Chunk* load_sfx(const char *path) {
    Mix_Chunk* sfx = nullptr;
    sfx = Mix_LoadWAV(path);
    
    if (sfx == nullptr) {
        show_sdl_error("Sound effect could not be loaded");
    }

    return sfx;
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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        show_sdl_error("SDL_mixer Initialization");
        return false;
    }

    return true;
}

int main(int argc, char **argv) {
    std::cout << "Path: " << argv[0] << std::endl;

    if (!init()) {
        return -1;
    }

    Window* window = new Window("WIDE BOI", SCREEN_W, SCREEN_H);
    if (!window->isCreated()) {
        return -1;
    }

    SDL_Renderer* renderer = window->getRenderer();
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    TextureManager* texManager = new TextureManager("assets/images", renderer);
    texManager->prefetch_directory(".png");
    texManager->fetch("static/base.png");

    SDL_Event e;
    std::set<SDL_Keycode> pressed;
    std::set<SDL_Keycode> just_pressed;
    
    Clock clock = Clock();
    // double duration = 1;
    // bool quit = false;
    // while (!quit) {
    //     //std::cout << "Frame Time: " << duration << " ms\n";

    //     just_pressed.clear();

    //     while (SDL_PollEvent(&e) != 0) { // events to process
    //         // test for clicked X
    //         if (e.type == SDL_QUIT) {
    //             quit = true;
    //         }
    //         // build pressed keys set
    //         else if (e.type == SDL_KEYDOWN) {
    //             //std::cout << "PRESS: " << SDL_GetKeyName(e.key.keysym.sym) << std::endl;
    //             pressed.insert(e.key.keysym.sym);
    //             just_pressed.insert(e.key.keysym.sym);
    //         } else if (e.type == SDL_KEYUP) {
    //             //std::cout << "RELEASE: " << SDL_GetKeyName(e.key.keysym.sym) << std::endl;
    //             if (pressed.count(e.key.keysym.sym) == 1) {
    //                 pressed.extract(e.key.keysym.sym);
    //             }
    //         }
    //     }

    //     // Clear screen
    //     SDL_RenderClear(renderer);
        
    //     // flip
    //     window->update();
    //     duration = clock.tick();
    // }

    delete texManager;

    delete window;
    IMG_Quit();
    SDL_Quit();

    return 0;
}