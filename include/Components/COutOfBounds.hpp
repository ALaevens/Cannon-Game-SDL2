#pragma once

#include "../Engine/ECS.hpp"
#include "Vector2.hpp"
#include "Components/CDestRect.hpp"
#include <cmath>

using namespace ECS;

struct COutOfBounds : Component {
    bool killOnEdge[4];
    CDestRect* cRect = nullptr;

    COutOfBounds(bool top, bool bottom, bool left, bool right) : killOnEdge{top, bottom, left, right} {}
    COutOfBounds() : COutOfBounds(true, true, true, true) {}

    void init() override {
        cRect = &entity->getComponent<CDestRect>();
    }

    void update(double ms) override {
        if (killOnEdge[0] && cRect->bottom() < 0)
            entity->destroy();

        if (killOnEdge[1] && cRect->top() > 720)
            entity->destroy();
        
        if (killOnEdge[2] && cRect->right() < 0)
            entity->destroy();

        if (killOnEdge[3] && cRect->left() > 1280)
            entity->destroy();
    }
};