#pragma once
#include "../Engine/ECS.hpp"
#include "Vector2.hpp"
#include "Components/CDestRect.hpp"

using namespace ECS;

struct CPosition : Component {
    Vector2<float> pos;
    CDestRect* cDest = nullptr;

    CPosition() : CPosition(0, 0) {}
    CPosition(float p_x, float p_y) : pos(p_x, p_y) {}

    void init() override {
        if (entity->hasComponent<CDestRect>())
            cDest = &entity->getComponent<CDestRect>();
    }

    void update(double ms) override {
        if (cDest) {
            cDest->setPos((int)pos.x, (int)pos.y);
        }
    }
};