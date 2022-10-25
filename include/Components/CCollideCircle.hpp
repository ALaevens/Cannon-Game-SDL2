#pragma once

#include "../Engine/ECS.hpp"
#include "Components/CDestRect.hpp"
#include "Vector2.hpp"

using namespace ECS;

struct CCollideCircle : Component {
    CDestRect* cRect = nullptr;

    int radius = -1;
    Vector2<int> middle;

    CCollideCircle(int p_radius) : radius(p_radius) {}

    void init() override {
        if (entity->hasComponent<CDestRect>())
            cRect = &entity->getComponent<CDestRect>();
    }

    void update(double ms) override {
        if (cRect) {
            middle = cRect->getAnchorPos(CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);
        }
    }
};