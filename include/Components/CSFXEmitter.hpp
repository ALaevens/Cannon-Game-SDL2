#pragma once
#include "../Engine/ECS.hpp"

using namespace ECS;

struct CSFXEmitter : Component {
    Mix_Chunk* chunk;

    CSFXEmitter(Mix_Chunk* p_chunk) : chunk(p_chunk) {}

    void play() {
        Mix_PlayChannel(-1, chunk, 0);
    }
};