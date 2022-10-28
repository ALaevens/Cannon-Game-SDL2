#include "Engine/Game.hpp"
#include "Scenes/CannonScene.hpp"
#include "Scenes/MenuScene.hpp"
#include "Scenes/ControlsScene.hpp"

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;


int main(int argc, char **argv) {

    fs::path binary = argv[0];
    binary.make_preferred().remove_filename();
    fs::current_path(binary);

    if (!init()) {
        return -1;
    }

    Game* new_game = new Game();
    new_game->add_scene("cannon", new CannonScene());
    new_game->add_scene("menu", new MenuScene());
    new_game->add_scene("controls", new ControlsScene());
    new_game->run("menu");
    delete new_game;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}