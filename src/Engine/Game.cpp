#include "Engine/Scene.hpp"
#include "Engine/Game.hpp"

Game::Game() {
    init();
}

Game::~Game() {
    for (const auto& [id, scene] : scenes) {
        delete scene;
    }

    delete texManager;
    delete sfxManager;
    delete fontManager;
    delete window;
    delete keyboard;
}

int Game::init() {
    window = new Window("Pixel Projectiles", SCREEN_W, SCREEN_H);
    if (!window->isCreated()) {
        return -1;
    }

    renderer = window->getRenderer();
    texManager = new TextureManager("assets/images", renderer);
    texManager->prefetch_directory(".png");

    sfxManager = new SFXManager("assets/sounds");
    texManager->prefetch_directory(".wav");

    fontManager = new FontManager("assets/fonts", renderer);

    keyboard = new Keyboard();

    std::srand(std::time(nullptr));

    return 1;
}

void Game::add_scene(std::string id, Scene* scene) {
    scene->attatch_game(this);
    scenes.emplace(std::pair<std::string, Scene*>{id, scene});
}

void Game::run(std::string enter) {
    std::string sceneID = enter;

    Scene* scene = nullptr;
    while (sceneID != "quit") {
        scene = scenes[sceneID];

        if (scene == nullptr) {
            sceneID = "quit";
        } else {
            std::cout << "Enter scene [" << sceneID << "]\n";
            sceneID = scene->run();
        }
    }
}

