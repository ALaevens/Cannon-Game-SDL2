#pragma once

#include "Engine/Scene.hpp"
#include "Engine/ECS.hpp"
#include "Components/CCannonController.hpp"
#include "Components/CFontDisplay.hpp"
#include <map>

class CannonScene : public Scene {
public:
    enum EntityGroup : std::size_t {
        // render layers
        RLBackground,
        RLBall,
        RLForeground,

        // groups
        GGround,
        GBall,
        GCoin
    };

    ~CannonScene() {}

    Entity& setup_cannon();
    Entity& createGroundTile(int x);
    void createBackground();
    Entity& createCoin();
    Entity& createBall(Vector2<float> vi, int x, int y);
    void createGroundTiles();
    void setupText();
    void setup();
    void handleEvents() override;
    void update(double ms) override;
    void render() override;
    std::string run() override;

private:
    int score = 0;
    int shotsLeft = 3;
    CCannonController* cannonController;
    CFontDisplay* scoreText;
    CFontDisplay* shotsText;
    Manager manager;

    std::string next_scene = "quit";
};