#include "Scenes/ControlsScene.hpp"
#include "Engine/Game.hpp"
#include "Engine/Clock.hpp"
#include <string>
#include <SDL.h>
#include "Components/CPosition.hpp"
#include "Components/CStaticTexture.hpp"
#include "Components/CButton.hpp"

void ControlsScene::handleEvents() {
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

void ControlsScene::update(double ms) {
    if (game->keyboard->isKeyJustPressed(SDLK_ESCAPE)) {
        running = false;
        next_scene = "menu";
    }

    if (backButton->justClicked) {
        running = false;
        next_scene = "menu";
    }

    manager.refresh();
    manager.update(ms);
}

void ControlsScene::render() {
    SDL_SetRenderDrawColor(game->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(game->renderer);

    manager.draw(game->renderer, manager.getEntitiesByGroup(EntityGroup::RLForeground));
    
    game->window->update();
}

std::string ControlsScene::run() {
    running = true;

    SDL_SetRenderDrawColor(game->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    ECS::Entity& backEntity = manager.addEntity();
    backEntity.addComponent<CDestRect>(SCREEN_W/2, 3*SCREEN_H/4 + 75, SCREEN_W/4, 70, CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);
    backButton = &backEntity.addComponent<CButton>("Go Back", SDL_Color{0x88, 0x00, 0x00, 0xFF}, game->fontManager->fetchFont("neoletters.ttf", 50), game->renderer);
    backEntity.addGroup(EntityGroup::RLForeground);

    ECS::Entity& imageEntity = manager.addEntity();
    imageEntity.addComponent<CDestRect>(CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);
    imageEntity.addComponent<CPosition>(SCREEN_W/2, SCREEN_H/3);
    imageEntity.addComponent<CStaticTexture>(game->texManager->fetch("static/controls.png"), 1);
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