#pragma once
#include "Engine/Scene.hpp"
#include "Engine/ECS.hpp"
#include "Components/CButton.hpp"

class MenuScene : public Scene {
public:
    enum EntityGroup : std::size_t {
        // render layers
        RLBackground,
        RLForeground
    };


    ~MenuScene() {}

    void handleEvents() override;
    void update(double ms) override;
    void render() override;

    std::string run() override;

private:
    std::string next_scene = "quit";
    ECS::Manager manager;
    CButton* startButton;
    CButton* controlsButton;
    
};