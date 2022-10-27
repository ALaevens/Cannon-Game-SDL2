#include "Scenes/CannonScene.hpp"
#include <SDL.h>
#include <Engine/Game.hpp>
#include <Engine/ECS.hpp>
#include <Engine/Clock.hpp>
#include <Components/Components.hpp>
#include "Components/CCannonController.hpp"
#include "consts.hpp"

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

Entity& CannonScene::setup_cannon() {
    auto& barrel = manager.addEntity();
    barrel.addComponent<CDestRect>();
    barrel.addComponent<CPosition>(32, SCREEN_H-96);
    barrel.addComponent<CRotation>(0, 32, 32);
    barrel.addComponent<CAnimatedTexture>(game->texManager->fetch("animated/barrel.png"), 5, 20.0, 4);
    barrel.addGroup(EntityGroup::RLForeground);

    auto& bar = manager.addEntity();
    bar.addComponent<CDestRect>(CDestRect::Anchor::LEFT | CDestRect::Anchor::BOTTOM);
    bar.addComponent<CPosition>(0, SCREEN_H-32);
    bar.addComponent<CStaticTexture>(game->texManager->fetch("static/meter.png"), 4);
    bar.addGroup(EntityGroup::RLForeground);

    auto& slider = manager.addEntity();
    slider.addComponent<CDestRect>(CDestRect::Anchor::LEFT | CDestRect::Anchor::CENTER_Y);
    slider.addComponent<CPosition>(0, SCREEN_H-32);
    slider.addComponent<CStaticTexture>(game->texManager->fetch("static/slider.png"), 2);
    slider.addGroup(EntityGroup::RLForeground);

    CMeterController& meter = bar.addComponent<CMeterController>(slider);

    auto& base = manager.addEntity();
    base.addComponent<CDestRect>();
    base.addComponent<CPosition>(32, SCREEN_H-96);
    base.addComponent<CStaticTexture>(game->texManager->fetch("static/base.png"), 4);

    std::vector<Mix_Chunk*> sounds;
    sounds.push_back(game->sfxManager->fetch_sfx("cannon.wav"));
    sounds.push_back(game->sfxManager->fetch_sfx("vine-boom.wav"));
    base.addComponent<CSFXEmitter>(sounds);

    base.addComponent<CCannonController>(game->keyboard, barrel, meter);
    base.addGroup(EntityGroup::RLForeground);

    return base;
}

Entity& CannonScene::createGroundTile(int x) {
    auto& entity = manager.addEntity();
    entity.addComponent<CDestRect>();
    entity.addComponent<CPosition>(x, SCREEN_H-32);
    entity.addComponent<CCollideRect>();
    entity.addComponent<CStaticTexture>(game->texManager->fetch("static/ground.png"), 4);
    entity.addGroup(EntityGroup::RLBackground);
    entity.addGroup(EntityGroup::GGround);

    return entity;
}

void CannonScene::createGroundTiles() {
    int offset = SCREEN_W % 64;

    for (int x = -(offset / 2); x < SCREEN_W; x+=64) {
        createGroundTile(x);
    }
}

void CannonScene::createBackground() {
    int x = 0;

    while (x < SCREEN_W -32) {
        auto& entity = manager.addEntity();
        entity.addComponent<CDestRect>(CDestRect::Anchor::BOTTOM | CDestRect::Anchor::LEFT);
        entity.addComponent<CPosition>(x, SCREEN_H-32);
        entity.addComponent<CStaticTexture>(game->texManager->fetch("static/tree.png"), (rand() % 10)/5 + 4);
        entity.addGroup(EntityGroup::RLBackground);

        x += (rand() % 128) + 32;
    }

    x = -16;

    while (x < SCREEN_W -16) {
        auto& entity = manager.addEntity();
        entity.addComponent<CDestRect>(CDestRect::Anchor::BOTTOM | CDestRect::Anchor::LEFT);
        entity.addComponent<CPosition>(x, SCREEN_H-32);
        entity.addComponent<CStaticTexture>(game->texManager->fetch("static/grass" + std::to_string(rand() % 3) + ".png"), 2);
        entity.addGroup(EntityGroup::RLBackground);

        x += (rand() % 64) + 32;
    }

    for (int i = 0; i < 10; i++) {
        auto& entity = manager.addEntity();
        entity.addComponent<CDestRect>(CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);
        entity.addComponent<CPosition>(rand() % (2 * SCREEN_W) - SCREEN_W, rand() % 256);
        entity.addComponent<CKinematics>(Vector2<float>{0, 0}, Vector2<float>{(float)(rand() % 40 + 10), 0});
        entity.addComponent<CCloudController>();
        entity.addComponent<CStaticTexture>(game->texManager->fetch("static/cloud" + std::to_string(rand() % 3) + ".png"), (rand() % 6)/2 + 8);
        entity.addGroup(EntityGroup::RLBackground);
    }
}

Entity& CannonScene::createCoin() {
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
    auto& animation = entity.addComponent<CAnimatedTexture>(game->texManager->fetch("animated/coin.png"), 8, 10.0, scale);
    animation.play(-1);

    std::vector<Mix_Chunk*> sounds;
    sounds.push_back(game->sfxManager->fetch_sfx("coin.wav"));
    entity.addComponent<CSFXEmitter>(sounds);

    entity.addGroup(EntityGroup::RLBall);
    entity.addGroup(EntityGroup::GCoin);

    return entity;
}

void CannonScene::setupText() {
    auto& scoreEntity = manager.addEntity();
    scoreText = &scoreEntity.addComponent<CFontDisplay>(game->fontManager->fetchFont("VCR_OSD.ttf", 30), game->renderer, "Score: 0", 0, 0);
    scoreText->setStyle(TextStyle{
        FontRenderStyle::BLENDED,
        SDL_Color{0xFF, 0x00, 0x00, 0xFF},
        SDL_Color{0xFF, 0x00, 0x00, 0xFF}
    });
    scoreEntity.addGroup(EntityGroup::RLForeground);

    auto& shotsEntity = manager.addEntity();
    shotsText = &shotsEntity.addComponent<CFontDisplay>(game->fontManager->fetchFont("VCR_OSD.ttf", 30), game->renderer, "Shots Left: 3", 0, 30);
    shotsText->setStyle(TextStyle{
        FontRenderStyle::BLENDED,
        SDL_Color{0x00, 0x00, 0x00, 0xFF},
        SDL_Color{0xFF, 0x00, 0x00, 0xFF}
    });
    shotsEntity.addGroup(EntityGroup::RLForeground);
}

Entity& CannonScene::createBall(Vector2<float> vi, int x, int y) {
    auto& entity = manager.addEntity();
    entity.addComponent<CDestRect>(CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);
    entity.addComponent<CPosition>(x, y);
    entity.addComponent<CRotation>(0, 16, 16);
    entity.addComponent<CKinematics>(Vector2<float>{0, 1800}, vi);
    entity.addComponent<CBallRotController>();
    entity.addComponent<CCollideRect>();
    entity.addComponent<CCollideCircle>(16);
    entity.addComponent<CStaticTexture>(game->texManager->fetch("static/ball.png"), 2);
    entity.addComponent<COutOfBounds>(false, true, true, true);
    entity.addComponent<CLifetime>(10);

    entity.addGroup(EntityGroup::RLBall);
    entity.addGroup(EntityGroup::GBall);

    return entity;
}

void CannonScene::setup() {
    Entity& cannonBase = setup_cannon();
    cannonController = &cannonBase.getComponent<CCannonController>();

    createGroundTiles();
    createBackground();
    createCoin();
    setupText();

    score = 0;
    shotsLeft = 3;
}

void CannonScene::handleEvents() {
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

void CannonScene::update(double ms) {
// allow user to quit with the escape key also
    if (game->keyboard->isKeyJustPressed(SDLK_ESCAPE)) {
        running = false;
        next_scene = "menu";

        // Mix_FadeOutMusic(250);
    }

    // press space to trigger a fire event
    if (game->keyboard->isKeyJustPressed(SDLK_SPACE) && shotsLeft >= 1) {
        // spawn cannonball with position and velocity from Cannon Controller
        int x = (int)cannonController->spawnLocation.x;
        int y = (int)cannonController->spawnLocation.y;
        createBall(cannonController->spawnVector, x, y);

        // update text on screen
        shotsLeft--;
        std::stringstream ss;
        ss << "Shots Left: " << shotsLeft;
        shotsText->setText(ss.str());
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
                scoreText->setText(ss.str());

                // play a sound the remove the coin
                CSFXEmitter& emitter = c->getComponent<CSFXEmitter>();
                CAnimatedTexture& tex = c->getComponent<CAnimatedTexture>();
                shotsLeft += 4 - ((int)tex.scale - 1);
                
                ss = std::stringstream();
                ss << "Shots Left: " << shotsLeft;
                shotsText->setText(ss.str());

                emitter.play(0);

                c->destroy();
                createCoin();
            }
        }
    }

    if (shotsLeft <= 0 && balls.size() < 1) {
        next_scene = "menu";
        running = false;
    }
}

void CannonScene::render() {
    SDL_RenderClear(game->renderer);

    // render in groups so that the cannon + ui is in front of the coins which are in front of the background
    manager.draw(game->renderer, manager.getEntitiesByGroup(EntityGroup::RLBackground));
    manager.draw(game->renderer, manager.getEntitiesByGroup(EntityGroup::RLBall));
    manager.draw(game->renderer, manager.getEntitiesByGroup(EntityGroup::RLForeground));
    
    
    game->window->update();
}

std::string CannonScene::run() {
    running = true;

    SDL_SetRenderDrawColor(game->renderer, 0x87, 0xCE, 0xFA, 0xFF);
    // Mix_PlayMusic(game->sfxManager->fetch_music("music.wav"), -1);

    setup();

    Clock clock{1.0}; // Minimum frame time = 1ms
    double duration = clock.tick(); // frame time in milliseconds

    while (running) {
        handleEvents();
        update(duration);           
        render();

        duration = clock.tick();
    }

    manager.empty();
    return next_scene;
}