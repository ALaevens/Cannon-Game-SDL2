#pragma once
#include "../Engine/ECS.hpp"
#include "Components/CDestRect.hpp"
#include "Components/CPosition.hpp"
#include "Components/CKinematics.hpp"
#include "consts.hpp"
#include <SDL.h>

using namespace ECS;

struct CCloudController : Component {
    CDestRect* cRect = nullptr;
    CPosition* cPos = nullptr;

    void init() override {
        cRect = &entity->getComponent<CDestRect>();
        cPos = &entity->getComponent<CPosition>();
    }

    void update(double ms) override {
        if (cRect->left() > SCREEN_W) {
            cPos->pos.x = -0.5*cRect->rect.w;
        }
    }
};