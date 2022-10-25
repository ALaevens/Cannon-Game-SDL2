#pragma once
#include "../Engine/ECS.hpp"
#include "Vector2.hpp"
#include "../Engine/Keyboard.hpp"
#include "game.hpp"
#include "consts.hpp"
#include <algorithm>
#include <cstdlib>

using namespace ECS;

class Game;

struct CCannonController : Component {
    Keyboard* keyboard;
    Entity& barrel;
    CMeterController& cMeter;
    CAnimatedTexture* cBarrelTex;
    CRotation* cRot;
    CSFXEmitter* cSFX;
    Vector2<float> spawnLocation;
    Vector2<float> spawnVector;

    CCannonController(Keyboard* p_keyboard, Entity& p_barrel, CMeterController& p_meter) : keyboard(p_keyboard), barrel(p_barrel), cMeter(p_meter) {}
    // CCannonController(std::function<void()> p_func, Keyboard* p_keyboard) : spawnBall(p_func), keyboard(p_keyboard) {}

    void init() override {
        cBarrelTex = &barrel.getComponent<CAnimatedTexture>();
        cRot = &barrel.getComponent<CRotation>();
        cSFX = &entity->getComponent<CSFXEmitter>();
    }

    void update(double ms) override {
        double sec = ms / 1000.0;

        if (keyboard->isKeyJustPressed(SDLK_SPACE)) {
            cBarrelTex->play(1);

            int r = std::rand() % 20;
            if (r >= 1)
                cSFX->play(0);
            else
                cSFX->play(1);
        }

        int moveDegrees = 75;
        if (keyboard->isKeyPressed(SDLK_LSHIFT))
            moveDegrees = 20;

        if (keyboard->isKeyPressed(SDLK_a)) {
            cRot->degrees -= sec * moveDegrees;
            cRot->degrees = std::max(cRot->degrees, -95.0);
            
        } else if (keyboard->isKeyPressed(SDLK_d)) {
            cRot->degrees += sec * moveDegrees;
            cRot->degrees = std::min(cRot->degrees, 5.0);
        }

        Vector2<float> textureStart = {(float)cBarrelTex->cDest->left(), (float)cBarrelTex->cDest->top()};
        Vector2<float> anchor = {(float)cRot->point()->x, (float)cRot->point()->y};
        Vector2<float> barrel;
        barrel.fromMagAngle(80, (cRot->degrees * PI)/180.0);
        spawnLocation = textureStart + anchor + barrel;
        spawnVector.fromMagAngle(500 + cMeter.power*1500.0, (cRot->degrees * PI)/180.0);
    }
};