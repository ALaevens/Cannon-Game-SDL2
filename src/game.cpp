#include "game.hpp"

#include "Engine/Engine.hpp"
#include "Components/Components.hpp"
#include "consts.hpp"
#include <ctime>
#include <cstdlib>
#include <cmath>

void collideGround(Entity& ball, Entity& ground) {
    auto& rectGround = ground.getComponent<CCollideRect>();
    auto& physBall = ball.getComponent<CKinematics>();
    auto& rectBall = ball.getComponent<CCollideRect>();
    auto& posBall = ball.getComponent<CPosition>();

    if (posBall.pos.y > rectGround.top() - rectBall.rect.h / 2) {
        posBall.pos.y = rectGround.top() - rectBall.rect.h / 2;
        posBall.update(0); // update position again so it doesnt appear to render in the ground
        
        physBall.v.x *= 0.8;
        if (abs(physBall.v.x) < 1) // eliminate constant slow rolling
            physBall.v.x = 0;

        if (physBall.v.y > 0) {
            physBall.v.y *= -0.8;

            // detect low-bounce jitter
            double peak_time = -physBall.v.y / physBall.a.y;
            double peak_height = physBall.v.y*peak_time + (0.5 * physBall.a.y * pow(peak_time, 2));
            if (peak_height > -1) {
                // once bounces become too small then stop y motion
                physBall.a.y = 0;
                physBall.v.y = 0;
            }
        }   
    }
}

bool collideCoin(Entity& ball, Entity& coin) {
    if (!coin.isAlive())
        return false;

    auto& ballCollider = ball.getComponent<CCollideCircle>();
    auto& coinCollider = coin.getComponent<CCollideCircle>();

    Vector2<int> diff = ballCollider.middle - coinCollider.middle;
    std::pair<float, float> magAngle = diff.toMagAngle();
    
    return (magAngle.first < (ballCollider.radius + coinCollider.radius));
}

Game::~Game() {
    delete texManager;
    delete sfxManager;
    delete fontManager;
    delete window;
    delete keyboard;
}

Entity& Game::createBall(Vector2<float> vi, int x, int y) {
    auto& entity = manager.addEntity();
    entity.addComponent<CDestRect>(CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);
    entity.addComponent<CPosition>(x, y);
    entity.addComponent<CRotation>(0, 16, 16);
    entity.addComponent<CKinematics>(Vector2<float>{0, 1800}, vi);
    entity.addComponent<CBallRotController>();
    entity.addComponent<CCollideRect>();
    entity.addComponent<CCollideCircle>(16);
    entity.addComponent<CStaticTexture>(texManager->fetch("static/ball.png"), 2);
    entity.addComponent<COutOfBounds>(false, true, true, true);
    entity.addComponent<CLifetime>(10);

    entity.addGroup(EntityGroup::RLBall);
    entity.addGroup(EntityGroup::GBall);

    return entity;
}

Entity& Game::createCoin() {
    int scale = (rand() % 3) + 2;

    auto& entity = manager.addEntity();
    entity.addComponent<CDestRect>(CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);

    int x = std::rand() % (SCREEN_W - 96 - (16* scale));
    x += 96;
    x += 8 * scale;
    int y = std::rand() % (SCREEN_H - 32 - (16* scale));
    y += 8 * scale;
    entity.addComponent<CPosition>(x, y);
    entity.addComponent<CCollideCircle>(8*scale);
    auto& animation = entity.addComponent<CAnimatedTexture>(texManager->fetch("animated/coin.png"), 8, 10.0, scale);
    animation.play(-1);

    std::vector<Mix_Chunk*> sounds;
    sounds.push_back(sfxManager->fetch_sfx("coin.wav"));
    entity.addComponent<CSFXEmitter>(sounds);

    entity.addGroup(EntityGroup::RLBall);
    entity.addGroup(EntityGroup::GCoin);

    return entity;
}

Entity& Game::setupCannon() {
    auto& barrel = manager.addEntity();
    barrel.addComponent<CDestRect>();
    barrel.addComponent<CPosition>(32, SCREEN_H-96);
    barrel.addComponent<CRotation>(0, 32, 32);
    barrel.addComponent<CAnimatedTexture>(texManager->fetch("animated/barrel.png"), 5, 20.0, 4);
    barrel.addGroup(EntityGroup::RLForeground);

    auto& bar = manager.addEntity();
    bar.addComponent<CDestRect>(CDestRect::Anchor::LEFT | CDestRect::Anchor::BOTTOM);
    bar.addComponent<CPosition>(0, SCREEN_H-32);
    bar.addComponent<CStaticTexture>(texManager->fetch("static/meter.png"), 4);
    bar.addGroup(EntityGroup::RLForeground);

    auto& slider = manager.addEntity();
    slider.addComponent<CDestRect>(CDestRect::Anchor::LEFT | CDestRect::Anchor::CENTER_Y);
    slider.addComponent<CPosition>(0, SCREEN_H-32);
    slider.addComponent<CStaticTexture>(texManager->fetch("static/slider.png"), 2);
    slider.addGroup(EntityGroup::RLForeground);

    CMeterController& meter = bar.addComponent<CMeterController>(slider);

    auto& base = manager.addEntity();
    base.addComponent<CDestRect>();
    base.addComponent<CPosition>(32, SCREEN_H-96);
    base.addComponent<CStaticTexture>(texManager->fetch("static/base.png"), 4);

    std::vector<Mix_Chunk*> sounds;
    sounds.push_back(sfxManager->fetch_sfx("cannon.wav"));
    sounds.push_back(sfxManager->fetch_sfx("vine-boom.wav"));
    base.addComponent<CSFXEmitter>(sounds);

    base.addComponent<CCannonController>(keyboard, barrel, meter);
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

void Game::createBackground() {
    int x = 0;

    while (x < SCREEN_W -32) {
        

        auto& entity = manager.addEntity();
        entity.addComponent<CDestRect>(CDestRect::Anchor::BOTTOM | CDestRect::Anchor::LEFT);
        entity.addComponent<CPosition>(x, SCREEN_H-32);
        entity.addComponent<CStaticTexture>(texManager->fetch("static/tree.png"), (rand() % 10)/5 + 4);
        entity.addGroup(EntityGroup::RLBackground);

        x += (rand() % 128) + 32;
    }

    x = -16;

    while (x < SCREEN_W -16) {
        auto& entity = manager.addEntity();
        entity.addComponent<CDestRect>(CDestRect::Anchor::BOTTOM | CDestRect::Anchor::LEFT);
        entity.addComponent<CPosition>(x, SCREEN_H-32);
        entity.addComponent<CStaticTexture>(texManager->fetch("static/grass" + std::to_string(rand() % 3) + ".png"), 2);
        entity.addGroup(EntityGroup::RLBackground);

        x += (rand() % 64) + 32;
    }

    for (int i = 0; i < 10; i++) {
        auto& entity = manager.addEntity();
        entity.addComponent<CDestRect>(CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);
        entity.addComponent<CPosition>(rand() % (2 * SCREEN_W) - SCREEN_W, rand() % 256);
        entity.addComponent<CKinematics>(Vector2<float>{0, 0}, Vector2<float>{(float)(rand() % 40 + 10), 0});
        entity.addComponent<CCloudController>();
        entity.addComponent<CStaticTexture>(texManager->fetch("static/cloud" + std::to_string(rand() % 3) + ".png"), (rand() % 6)/2 + 8);
        // entity.addComponent<CStaticTexture>(texManager->fetch("static/cloud2.png"), 8);
        entity.addGroup(EntityGroup::RLBackground);
    }
}

std::map<std::string, CFontDisplay&> Game::setupText() {
    std::map<std::string, CFontDisplay&> texts;

    auto& scoreEntity = manager.addEntity();
    CFontDisplay& scoreComp = scoreEntity.addComponent<CFontDisplay>(fontManager->fetchFont("VCR_OSD.ttf", 30), renderer, "Score: 0", 0, 0);
    scoreComp.setStyle(TextStyle{
        FontRenderStyle::BLENDED,
        SDL_Color{0xFF, 0x00, 0x00, 0xFF},
        SDL_Color{0xFF, 0x00, 0x00, 0xFF}
    });
    scoreEntity.addGroup(EntityGroup::RLForeground);
    texts.emplace(std::pair<std::string, CFontDisplay&>{"score", scoreComp});

    auto& shotsEntity = manager.addEntity();
    CFontDisplay& shotsComp = shotsEntity.addComponent<CFontDisplay>(fontManager->fetchFont("VCR_OSD.ttf", 30), renderer, "Shots Fired: 0", 0, 30);
    scoreComp.setStyle(TextStyle{
        FontRenderStyle::BLENDED,
        SDL_Color{0xFF, 0x00, 0x00, 0xFF},
        SDL_Color{0xFF, 0x00, 0x00, 0xFF}
    });
    shotsEntity.addGroup(EntityGroup::RLForeground);
    texts.emplace(std::pair<std::string, CFontDisplay&>{"shots", shotsComp});

    return texts;
}

void Game::createGroundTiles() {
    int offset = SCREEN_W % 64;

    for (int x = -(offset / 2); x < SCREEN_W; x+=64) {
        createGroundTile(x);
    }
}

int Game::startup() {
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

    fontManager = new FontManager("assets/fonts", renderer);

    keyboard = new Keyboard();

    std::srand(std::time(nullptr));

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

Uint64 Game::update(double ms, CCannonController& cannonController, std::map<std::string, CFontDisplay&>* uiTexts) {
    Uint64 start = SDL_GetTicks64();

    // allow user to quit with the escape key also
    if (keyboard->isKeyPressed(SDLK_ESCAPE)) {
        running = false;
    }

    // press space to trigger a fire event
    if (keyboard->isKeyJustPressed(SDLK_SPACE)) {
        // spawn cannonball with position and velocity from Cannon Controller
        int x = (int)cannonController.spawnLocation.x;
        int y = (int)cannonController.spawnLocation.y;
        createBall(cannonController.spawnVector, x, y);

        // update text on screen
        shotsFired++;
        std::stringstream ss;
        ss << "Shots Fired: " << shotsFired;
        uiTexts->at("shots").setText(ss.str());
    }

    manager.refresh();
    manager.update(ms);

    // detect collisions
    auto& balls = manager.getEntitiesByGroup(EntityGroup::GBall);
    auto& ground = manager.getEntitiesByGroup(EntityGroup::GGround);
    auto& coins = manager.getEntitiesByGroup(EntityGroup::GCoin);

    for (auto& b : balls) {
        for (auto& g : ground) 
            collideGround(*b, *g);

        for (auto& c : coins) {
            if (collideCoin(*b, *c)) {
                // update text on screen
                score += 1;
                std::stringstream ss;
                ss << "Score: " << score;
                uiTexts->at("score").setText(ss.str());

                // play a sound the remove the coin
                CSFXEmitter& emitter = c->getComponent<CSFXEmitter>();
                emitter.play(0);

                c->destroy();
                createCoin();
            }
        }
    }

    Uint64 end = SDL_GetTicks64();
    return end - start;
}

Uint64 Game::render() {
    Uint64 start = SDL_GetTicks64();
    
    SDL_RenderClear(renderer);

    // render in groups so that the cannon + ui is in front of the coins which are in front of the background
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

    createGroundTiles();
    createBackground();
    createCoin();

    std::map<std::string, CFontDisplay&> uiTexts = setupText();

    Mix_PlayMusic(sfxManager->fetch_music("music.wav"), -1);

    Clock clock{1.0}; // Minimum frame time = 1ms
    double duration = clock.tick(); // frame time in milliseconds

    std::stringstream ss;
    std::string title;

    while (running) {
        handleEvents();
        update(duration, cannonController, &uiTexts);           
        render();

        // statistics
        duration = clock.tick();
        ss.str("");
        ss.clear();
        ss << "FPS: " << floor(1000.0 / (duration));
        title = ss.str();
        window->set_title(title.c_str());
    }
}