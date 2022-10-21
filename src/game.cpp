#include "game.hpp"

#include "Engine/Engine.hpp"
#include "Components/Components.hpp"
#include "consts.hpp"

void collideGround(Entity& ball, Entity& ground) {
    auto& rectGround = ground.getComponent<CCollideRect>();
    auto& physBall = ball.getComponent<CKinematics>();
    auto& rectBall = ball.getComponent<CCollideRect>();
    auto& posBall = ball.getComponent<CPosition>();

    if (rectBall.bottom() > rectGround.top()) {
        posBall.pos.y = rectGround.top() - rectBall.rect.h / 2;
        posBall.update(0);

        if (physBall.v.y > 0) {
            physBall.v = physBall.v * Vector2<float>{0.8, -0.8};
            if (physBall.v.y > -100 && physBall.v.y < 100) {
                physBall.v.y = 0;
                physBall.a.y = 0;
            }
        }   
    }
}

Game::~Game() {
    delete texManager;
    delete sfxManager;
    delete window;
    delete keyboard;
}

Entity& Game::createBall(Vector2<float> vi, int x, int y) {
    auto& entity = manager.addEntity();
    entity.addComponent<CDestRect>(CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);
    entity.addComponent<CPosition>(x, y);

    // Vector2<float> velocity;
    // velocity.fromMagAngle(1500, -0.45*PI);

    entity.addComponent<CKinematics>(Vector2<float>{0, 2500}, vi);
    entity.addComponent<CCollideRect>();
    entity.addComponent<CStaticTexture>(texManager->fetch("static/ball.png"), 2);
    entity.addComponent<COutOfBounds>(false, true, true, true);
    entity.addComponent<CLifetime>(10);

    entity.addGroup(EntityGroup::RLBall);

    return entity;
}

Entity& Game::setupCannon() {
    auto& barrel = manager.addEntity();
    barrel.addComponent<CDestRect>();
    barrel.addComponent<CPosition>(0, SCREEN_H-96);
    barrel.addComponent<CRotation>(0, 32, 32);
    barrel.addComponent<CAnimatedTexture>(texManager->fetch("animated/barrel.png"), 5, 20.0, 4);
    barrel.addGroup(EntityGroup::RLForeground);

    auto& base = manager.addEntity();
    base.addComponent<CDestRect>();
    base.addComponent<CPosition>(0, SCREEN_H-96);
    base.addComponent<CStaticTexture>(texManager->fetch("static/base.png"), 4);
    base.addComponent<CSFXEmitter>(sfxManager->fetch_sfx("cannon.wav"));
    base.addComponent<CCannonController>(keyboard, barrel);
    base.addGroup(EntityGroup::RLForeground);

    return base;
}

Entity& Game::createGroundTile(int x) {
    auto& entity = manager.addEntity();
    entity.addComponent<CDestRect>();
    entity.addComponent<CPosition>(x, SCREEN_H-32);
    entity.addComponent<CCollideRect>();
    entity.addComponent<CStaticTexture>(texManager->fetch("static/ground.png"), 4);
    entity.addGroup(EntityGroup::RLBackground);
    entity.addGroup(EntityGroup::GGround);

    return entity;
}

void Game::createGroundTiles() {
    int offset = SCREEN_W % 64;

    for (int x = -(offset / 2); x < SCREEN_W; x+=64) {
        createGroundTile(x);
    }
}

int Game::startup() {
    if (!init()) {
        return -1;
    }

    window = new Window("Cannon Game", SCREEN_W, SCREEN_H);
    if (!window->isCreated()) {
        return -1;
    }

    renderer = window->getRenderer();
    SDL_SetRenderDrawColor(renderer, 0x87, 0xCE, 0xFA, 0xFF);
    texManager = new TextureManager("assets/images", renderer);
    texManager->prefetch_directory(".png");

    sfxManager = new SFXManager("assets/sounds");
    texManager->prefetch_directory(".wav");

    keyboard = new Keyboard();

    createGroundTiles();

    return 1;
}

Uint64 Game::handleEvents() {
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

Uint64 Game::update(double ms, CCannonController& cannonController) {
    Uint64 start = SDL_GetTicks64();

    if (keyboard->isKeyPressed(SDLK_ESCAPE)) {
        running = false;
    }

    if (keyboard->isKeyJustPressed(SDLK_SPACE)) {
        int x = (int)cannonController.spawnLocation.x;
        int y = (int)cannonController.spawnLocation.y;
        createBall(cannonController.spawnVector, x, y);
    }

    manager.refresh();
    manager.update(ms);

    auto& balls = manager.getEntitiesByGroup(EntityGroup::RLBall);
    auto& ground = manager.getEntitiesByGroup(EntityGroup::GGround);

    for (auto& b : balls) {
        for (auto& g : ground) 
            collideGround(*b, *g);
    }

    Uint64 end = SDL_GetTicks64();
    return end - start;
}

Uint64 Game::render() {
    Uint64 start = SDL_GetTicks64();
    
    SDL_RenderClear(renderer);

    manager.draw(renderer, manager.getEntitiesByGroup(EntityGroup::RLBackground));
    manager.draw(renderer, manager.getEntitiesByGroup(EntityGroup::RLBall));
    manager.draw(renderer, manager.getEntitiesByGroup(EntityGroup::RLForeground));
    
    
    window->update();

    Uint64 end = SDL_GetTicks64();
    return end - start;
}

void Game::run() {
    running = true;

    if (!startup()) running = false;

    Entity& cannonBase = setupCannon();
    CCannonController& cannonController = cannonBase.getComponent<CCannonController>();

    Clock clock{1.0}; // Minimum frame time = 1ms
    double duration = 0;
    Uint64 ms_events, ms_update, ms_render;

    std::stringstream ss;
    std::string title;
    while (running) {
        ms_events = handleEvents();
        ms_update = update(duration, cannonController);           
        ms_render = render();

        // statistics
        duration = clock.tick();
        ss.str("");
        ss.clear();
        // ss << "Frame Time: " << duration;
        ss << "FPS: " << floor(1000.0 / (duration));
        title = ss.str();
        window->set_title(title.c_str());
        // if (keyboard->isKeyPressed(SDLK_f)) {
        //     std::cout << "Frame Time: " << duration << " ms\n";
            
        //     if (keyboard->isKeyPressed(SDLK_LSHIFT)) {
        //         std::cout << "  Event Loop: " << ms_events << " ms\n";
        //         std::cout << "  Update:     " << ms_update << " ms\n";
        //         std::cout << "  Render:     " << ms_render << " ms\n";
        //     }
        // }
    }
}