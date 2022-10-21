#pragma once

#include "Engine/Engine.hpp"
#include "Components/Components.hpp"
#include "Components/CCannonController.hpp"
#include "consts.hpp"

void collideGround(Entity& ball, Entity& ground);

class Game {
public:
    enum EntityGroup : std::size_t {
        // render layers
        RLBackground,
        RLBall,
        RLForeground,

        // groups
        GGround
    };


    ~Game();

    Entity& createBall(Vector2<float> vi, int x, int y);

    Entity& setupCannon();

    Entity& createGroundTile(int x);

    void createGroundTiles();

    int startup();

    Uint64 handleEvents();

    Uint64 update(double ms, CCannonController& cannonController);

    Uint64 render();

    void run();


private:
    Window* window;
    SDL_Renderer* renderer;
    TextureManager* texManager;
    SFXManager* sfxManager;
    Keyboard* keyboard;
    Manager manager;
    bool running = false;
};