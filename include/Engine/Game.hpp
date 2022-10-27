#ifndef GAME_H
#define GAME_H

#include "Engine/Window.hpp"
#include "Engine/TextureManager.hpp"
#include "Engine/SFXManager.hpp"
#include "Engine/FontManager.hpp"
#include "Engine/Keyboard.hpp"

#include <map>
#include <string>

class Scene;

class Game {
public:
    Game();
    ~Game();

    int init();
    void run(std::string enter);
    void add_scene(std::string id, Scene* scene);

    Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TextureManager* texManager = nullptr;
    SFXManager* sfxManager = nullptr;
    FontManager* fontManager = nullptr;
    Keyboard* keyboard = nullptr;

private:
    std::map<std::string, Scene*> scenes;
};

#endif