#include "Scenes/MenuScene.hpp"
#include "Engine/Game.hpp"
#include "Engine/Clock.hpp"
#include <string>
#include <SDL.h>
#include "Components/CPosition.hpp"
#include "Components/CStaticTexture.hpp"
#include "Components/CButton.hpp"

void MenuScene::handleEvents() {
    SDL_Event e;
    game->keyboard->refresh();

    while (SDL_PollEvent(&e) != 0) { // events to process
        game->keyboard->updateKeys(&e);

        // test for clicked X
        if (e.type == SDL_QUIT) {
            running = false;
            next_scene = "quit";
        }
    }
}

void MenuScene::update(double ms) {
    if (game->keyboard->isKeyJustPressed(SDLK_ESCAPE)) {
        running = false;
        next_scene = "quit";
    }

    if (game->keyboard->isKeyJustPressed(SDLK_SPACE)) {
        running = false;
        next_scene = "cannon";
    }

    if (startButton->justClicked) {
        running = false;
        next_scene = "cannon";
    }

    if (controlsButton->justClicked) {
        running = false;
        next_scene = "controls";
    }

    manager.refresh();
    manager.update(ms);
}

void MenuScene::render() {
    SDL_SetRenderDrawColor(game->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(game->renderer);

    // boxColor(game->renderer, 10, 10, 100, 100, 0xFF0000FF);

    manager.draw(game->renderer, manager.getEntitiesByGroup(EntityGroup::RLBackground));
    manager.draw(game->renderer, manager.getEntitiesByGroup(EntityGroup::RLForeground));
    
    game->window->update();
}

std::string MenuScene::run() {
    running = true;

    SDL_SetRenderDrawColor(game->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    ECS::Entity& startEntity = manager.addEntity();
    startEntity.addComponent<CDestRect>(SCREEN_W/3, 3*SCREEN_H/4, SCREEN_W/4, 70, CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);
    startButton = &startEntity.addComponent<CButton>("Start Game", SDL_Color{0x00, 0x5D, 0x8F, 0xFF}, game->fontManager->fetchFont("neoletters.ttf", 50), game->renderer);
    startEntity.addGroup(EntityGroup::RLForeground);

    ECS::Entity& controlsEntity = manager.addEntity();
    controlsEntity.addComponent<CDestRect>(2*SCREEN_W/3, 3*SCREEN_H/4, SCREEN_W/4, 70, CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);
    controlsButton = &controlsEntity.addComponent<CButton>("Controls", SDL_Color{0x00, 0x8F, 0x28, 0xFF}, game->fontManager->fetchFont("neoletters.ttf", 50), game->renderer);
    controlsEntity.addGroup(EntityGroup::RLForeground);

    ECS::Entity& imageEntity = manager.addEntity();
    imageEntity.addComponent<CDestRect>(CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);
    imageEntity.addComponent<CPosition>(SCREEN_W/2, SCREEN_H/3);
    imageEntity.addComponent<CStaticTexture>(game->texManager->fetch("static/title.png"), 1);
    imageEntity.addGroup(EntityGroup::RLForeground);

    Clock clock{1.0};
    double duration = clock.tick();

    while (running) {
        handleEvents();
        update(duration);
        render();

        duration = clock.tick();
    }

    manager.empty();
    return next_scene;
}