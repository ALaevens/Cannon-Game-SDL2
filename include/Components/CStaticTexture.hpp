#pragma once
#include "../Engine/ECS.hpp"
#include "Components/CPosition.hpp"
#include "Components/CDestRect.hpp"

using namespace ECS;

struct CStaticTexture : Component {
    SDL_Texture* tex;
    SDL_Rect srcRect;
    CPosition* cPos = nullptr;
    CDestRect* cDest = nullptr;

    float scale;

    CStaticTexture(SDL_Texture *p_tex, float p_scale) : tex(p_tex), scale(p_scale) {}

    void init() override {
        cPos = &entity->getComponent<CPosition>();
        cDest = &entity->getComponent<CDestRect>();

        int w, h;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        srcRect.x = srcRect.y = 0;
        srcRect.w = w; srcRect.h = h;

        cDest->rect.w = w*scale;
        cDest->rect.h = h*scale;
    }

    void update(double ms) override {
        
    }

    void draw(SDL_Renderer* renderer) override {
        SDL_RenderCopy(renderer, tex, &srcRect, &cDest->rect);
    }
};