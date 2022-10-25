#pragma once

#include "../Engine/ECS.hpp"
#include "Vector2.hpp"
#include "Components/CPosition.hpp"
#include <cmath>
#include <iostream>

using namespace ECS;

struct CKinematics : Component {
    Vector2<float> a;
    Vector2<float> v;
    float maxV;

    CKinematics(Vector2<float> p_a, Vector2<float> p_v) : a(p_a), v(p_v) {}
    CKinematics() : CKinematics(Vector2<float>{0, 0}, Vector2<float>{0, 0}) {}

    CPosition* cPos = nullptr;
    void init() override {
        cPos = &entity->getComponent<CPosition>();
    }

    void update(double ms) override {
        double sec = ms / 1000.0;
        Vector2<float> v_f = (a*sec) + v;
        Vector2<float> d = (v*sec) + (a*0.5*pow(sec, 2));

        // std::cout << "vi: " << v.y << ", vf: " << v_f.y << std::endl;

        v = v_f;
        cPos->pos = cPos->pos + d;
    }
};