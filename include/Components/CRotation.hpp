#pragma once
#include "../Engine/ECS.hpp"
#include <SDL.h>

using namespace ECS;

struct CRotation : Component {
    double degrees;
    SDL_Point pt;

    CRotation(double p_degrees, int p_x, int p_y) : degrees(p_degrees), pt{p_x, p_y} {}
    CRotation(double p_degrees) : CRotation(p_degrees, -1, -1) {}
    CRotation() : CRotation(0) {}

    SDL_Point* point() {
        if (pt.x < 0 || pt.y < 0) {
            return nullptr;
        } else {
            return &pt;
        }
    }

    void init() override {

    }

    void update(double ms) override {

    }
};