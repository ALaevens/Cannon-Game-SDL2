#pragma once
#include "../Engine/ECS.hpp"
#include "Components/CPosition.hpp"
#include "consts.hpp"
#include <cmath>


using namespace ECS;

class Game;

struct CMeterController : Component {
    Entity& slider;
    CPosition* cPos;
    CDestRect* cRect;

    double power = 0.0;
    double time = 0.0;

    CMeterController(Entity& p_slider) : slider(p_slider) {}

    void init() override {
        cPos = &slider.getComponent<CPosition>();
        cRect = &entity->getComponent<CDestRect>();
    }

    void update(double ms) override {
        time += (ms / 1000.0);
        power = (0.5*cos(2*time))+0.5;

        cPos->pos.y = ((float)cRect->bottom() - power*cRect->rect.h);
    }
};