#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <iostream>
#include <Engine/ECS.hpp>

class Game;

class Scene {
public:
    virtual ~Scene() {}
    
    virtual void handleEvents() {};
    virtual void update(double ms) {};
    virtual void render() {};
    virtual std::string run() {
        return "quit";
    };

    void attatch_game(Game* p_game) {
        game = p_game;
    };

protected:
    Game* game;
    bool running = false;
};

#endif