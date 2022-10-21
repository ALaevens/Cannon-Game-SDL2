#pragma once
#include "../Engine/ECS.hpp"

using namespace ECS;

struct CLifetime : Component {
    double maxLifetime; 
    double lifetime;

    CLifetime(double p_maxLifetime) : maxLifetime(p_maxLifetime), lifetime(0.0f) {}
    CLifetime() : CLifetime(-1.0f) {}

    void update(double ms) override {
        lifetime += ms / 1000.0f;

        if (maxLifetime > 0 && lifetime > maxLifetime)
            entity->destroy();
    }

};