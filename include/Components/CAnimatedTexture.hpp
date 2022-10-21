#pragma once
#include "../Engine/ECS.hpp"
#include "Components/CPosition.hpp"
#include "Components/CDestRect.hpp"
#include "Components/CRotation.hpp"

using namespace ECS;

struct CAnimatedTexture : Component {
    SDL_Texture* tex;
    SDL_Rect srcRect;
    CDestRect* cDest = nullptr;
    CRotation* cRot = nullptr;
    float scale;
    int frames;
    int currentFrame;
    int fps;
    int plays;
    double timer;

    CAnimatedTexture(SDL_Texture *p_tex, int p_frames, int p_fps, float p_scale) : tex(p_tex), scale(p_scale), frames(p_frames), fps(p_fps), currentFrame(0), plays(0) {}

    void init() override {
        cDest = &entity->getComponent<CDestRect>();
        if (entity->hasComponent<CRotation>())
            cRot = &entity->getComponent<CRotation>();

        int w, h;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        w /= frames;
        srcRect.x = srcRect.y = 0;
        srcRect.w = w; srcRect.h = h;

        cDest->rect.w = w*scale;
        cDest->rect.h = h*scale;

        timer = 0.0;
    }

    void update(double ms) override {
        if (plays != 0) {
            timer += (ms / 1000.0);

            if (timer > double(1.0 / fps)) { // advance frame
                currentFrame++;
                currentFrame %= frames;
                
                if (currentFrame == 0)
                    plays--;

                // std::cout << "FRAME: " << currentFrame << std::endl;
                srcRect.x = currentFrame*srcRect.w;

                timer = 0.0;
            }
        }
    }

    void play(int n) {
        plays = n;
        timer = 0.0;
        currentFrame = 1;
        srcRect.x = currentFrame*srcRect.w;
    }

    void draw(SDL_Renderer* renderer) override {
        if (cRot)
            SDL_RenderCopyEx(renderer, tex, &srcRect, &cDest->rect, cRot->degrees, cRot->point(), SDL_FLIP_NONE);
        else
            SDL_RenderCopy(renderer, tex, &srcRect, &cDest->rect);
    }
};