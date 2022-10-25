#pragma once
#include "../Engine/ECS.hpp"
#include "Components/CRotation.hpp"
#include "Components/CKinematics.hpp"
#include "consts.hpp"
#include <SDL.h>

using namespace ECS;

struct CBallRotController : Component {
    CRotation* cRot = nullptr;
    CKinematics* cKin = nullptr;

    void init() override {
        cRot = &entity->getComponent<CRotation>();
        cKin = &entity->getComponent<CKinematics>();
    }

    void update(double ms) override {
        double sec = ms / 1000.0;
        double rot = (cKin->v.x*sec*360) / (2*PI*16);
        
        cRot->degrees += rot;
    }
};