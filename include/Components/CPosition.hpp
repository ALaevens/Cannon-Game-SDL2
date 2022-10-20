#pragma once
#include "../Engine/ECS.hpp"
#include "Vector2.hpp"

using namespace ECS;

struct CPosition : Component {
    Vector2<float> pos;

    CPosition() : CPosition(0, 0) {}
    CPosition(float p_x, float p_y) : pos(p_x, p_y) {}
};