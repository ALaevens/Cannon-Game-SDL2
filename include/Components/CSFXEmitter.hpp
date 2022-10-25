#pragma once
#include "../Engine/ECS.hpp"
#include <vector>

using namespace ECS;

struct CSFXEmitter : Component {
    std::vector<Mix_Chunk*> sounds;
    CSFXEmitter(std::vector<Mix_Chunk*> p_sounds) : sounds(p_sounds) {}

    // Mix_Chunk* &sounds;
    // CSFXEmitter(Mix_Chunk* &p_sounds) : sounds(p_sounds) {};

    void play(int soundID) {
        Mix_PlayChannel(-1, sounds.at(soundID), 0);
    }
};