#include "Engine/Engine.hpp"
#include "Components/Components.hpp"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <map>
#include <set>
#include <string>

const int SCREEN_W = 1280;
const int SCREEN_H = 720;
const float PI = 3.14159265359;

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

class Keyboard {
public:
    void updateKeys(SDL_Event* e) {
        if (e->type == SDL_KEYDOWN && pressed.count(e->key.keysym.sym) == 0) {
            std::cout << "PRESS: " << SDL_GetKeyName(e->key.keysym.sym) << std::endl;
            just_pressed.insert(e->key.keysym.sym);
            
            pressed.insert(e->key.keysym.sym);
            
        } else if (e->type == SDL_KEYUP) {
            std::cout << "RELEASE: " << SDL_GetKeyName(e->key.keysym.sym) << std::endl;
            if (pressed.count(e->key.keysym.sym) == 1) {
                pressed.extract(e->key.keysym.sym);
            }
        }
    }

    void refresh() {
        just_pressed.clear();
    }

    bool isKeyJustPressed(const SDL_Keycode key) {
        return just_pressed.count(key);
        return false;
    }

    bool isKeyPressed(const SDL_Keycode key) {
        return pressed.count(key);
        return false;
    }

private:
    std::set<SDL_Keycode> pressed;
    std::set<SDL_Keycode> just_pressed;
};

class Game {
public:
    int startup() {
        if (!init()) {
            return -1;
        }

        window = new Window("Cannon Game", SCREEN_W, SCREEN_H);
        if (!window->isCreated()) {
            return -1;
        }

        renderer = window->getRenderer();
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        texManager = new TextureManager("assets/images", renderer);
        texManager->prefetch_directory(".png");

        keyboard = new Keyboard();

        return 1;
    }

    ~Game() {
        delete texManager;
        delete window;
        delete keyboard;
    }

    Entity& createCannon() {
        auto& entity = manager.addEntity();
        entity.addComponent<CPosition>();
        entity.addComponent<CStaticTexture>(texManager->fetch("static/base.png"), 4);

        return entity;
    }

    Entity& createBall() {
        auto& entity = manager.addEntity();
        entity.addComponent<CPosition>(0, SCREEN_H-32);

        Vector2<float> velocity;
        velocity.fromMagAngle(300, -PI/3);

        entity.addComponent<CKinematics>(Vector2<float>{0, 50}, velocity);
        entity.addComponent<CStaticTexture>(texManager->fetch("static/ball.png"), 2);

        return entity;
    }

    Uint64 handleEvents() {
        Uint64 start = SDL_GetTicks64();

        SDL_Event e;
        keyboard->refresh();

        while (SDL_PollEvent(&e) != 0) { // events to process
            keyboard->updateKeys(&e);

            // test for clicked X
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        Uint64 end = SDL_GetTicks64();
        return end - start;
    }

    Uint64 update(Uint64 ms) {
        Uint64 start = SDL_GetTicks64();

        if (keyboard->isKeyPressed(SDLK_ESCAPE)) {
            running = false;
        }

        if (keyboard->isKeyJustPressed(SDLK_SPACE)) {
            createBall();
        }        

        manager.refresh();
        manager.update(ms);

        Uint64 end = SDL_GetTicks64();
        return end - start;
    }

    Uint64 render() {
        Uint64 start = SDL_GetTicks64();
        
        SDL_RenderClear(renderer);
        manager.draw(renderer);

        Uint64 end = SDL_GetTicks64();
        return end - start;
    }

    void run() {
        running = true;

        if (!startup()) running = false;

        Clock clock(1); // Minimum frame time = 1ms
        Uint64 duration = 0;
        Uint64 ms_events, ms_update, ms_render;

        while (running) {
            ms_events = handleEvents();
            ms_update = update(duration);           
            ms_render = render();

            // flip
            window->update();

            // statistics
            duration = clock.tick();

            if (keyboard->isKeyPressed(SDLK_f)) {
                std::cout << "Frame Time: " << duration << " ms\n";
                
                if (keyboard->isKeyPressed(SDLK_LSHIFT)) {
                    std::cout << "  Event Loop: " << ms_events << " ms\n";
                    std::cout << "  Update:     " << ms_update << " ms\n";
                    std::cout << "  Render:     " << ms_render << " ms\n";
                }
            }
        }
    }


private:
    Window* window;
    SDL_Renderer* renderer;
    TextureManager* texManager;
    Keyboard* keyboard;
    Manager manager;
    bool running = false;
};


int main(int argc, char **argv) {
    std::cout << "Path: " << argv[0] << std::endl;

    Game* game = new Game();
    game->run();
    delete game;

    IMG_Quit();
    SDL_Quit();

    return 0;
}