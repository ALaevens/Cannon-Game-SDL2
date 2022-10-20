#pragma once
#include "../Engine/ECS.hpp"
#include "Components/CPosition.hpp"

using namespace ECS;

struct CStaticTexture : Component {
    SDL_Texture* tex;
    SDL_Rect srcRect, destRect;
    CPosition* cPos = nullptr;
    float scale;

    CStaticTexture(SDL_Texture *p_tex, float p_scale) : tex(p_tex), scale(p_scale) {}

    void init() override {
        cPos = &entity->getComponent<CPosition>();

        int w, h;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        srcRect.x = srcRect.y = 0;
        srcRect.w = w; srcRect.h = h;
    }

    void update(float ms) override {
        destRect.w = srcRect.w*scale;
        destRect.h = srcRect.h*scale;
        destRect.x = cPos->pos.x;
        destRect.y = cPos->pos.y;
    }

    void draw(SDL_Renderer* renderer) override {
        SDL_RenderCopy(renderer, tex, &srcRect, &destRect);
    }
};