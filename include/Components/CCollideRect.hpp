#pragma once

#include "../Engine/ECS.hpp"
#include "Vector2.hpp"
#include "Components/CDestRect.hpp"
#include <SDL.h>
#include <cmath>

using namespace ECS;

struct CCollideRect : Component {
    CDestRect* cRect = nullptr;

    SDL_Rect rect;

    CCollideRect(SDL_Rect p_rect) : rect(p_rect) {}
    CCollideRect() : CCollideRect(SDL_Rect{0, 0, 1, 1}) {}

    int top() { return rect.y; }
    int bottom() { return rect.y + rect.h; }
    int left() { return rect.x; }
    int right() {return rect.x + rect.w; }
    
    Vector2<int> middle() {
        Vector2<int> res{(rect.x + rect.w) / 2, (rect.y + rect.h) / 2};
        return res;
    }

    void init() override {
        if (entity->hasComponent<CDestRect>())
            cRect = &entity->getComponent<CDestRect>();
    }

    void update(double ms) override {
        if (cRect) {
            rect = cRect->rect;
        }
    }
};