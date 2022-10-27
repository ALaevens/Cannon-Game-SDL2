#pragma once

#include "../Engine/ECS.hpp"
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <Components/CDestRect.hpp>
#include "Engine/FontManager.hpp"
#include <string>
#include "Vector2.hpp"
#include <algorithm>

using namespace ECS;

struct CButton : Component {
    CDestRect* cRect = nullptr;
    SDL_Color base_color;
    SDL_Color color;
    std::string text;
    TextStyle style = {
        FontRenderStyle::BLENDED,
        SDL_Color{0x00, 0x00, 0x00, 0xFF},
        SDL_Color{0x00, 0x00, 0x00, 0xFF}
    };
    bool changed = true;
    bool justClicked = false;

    FontTexture* fontTex = nullptr;

    CButton(std::string p_text, SDL_Color p_color, TTF_Font* font, SDL_Renderer* renderer) {
        base_color = p_color;
        color = base_color;
        fontTex = new FontTexture(font, renderer);
        setText(p_text);
    }

    ~CButton() {
        delete fontTex;
    }

    void setText(std::string p_text) {
        text = p_text;
        changed = true;
    }

    void setStyle(TextStyle p_style) {
        style = p_style;
        changed = true;
    }

    void init() override {
        cRect = &entity->getComponent<CDestRect>();
    }

    void update(double ms) override {
        if (changed) {
            fontTex->generate(text, style);
            changed = false;
        }

        int x, y;
        Uint32 mouse = SDL_GetMouseState(&x, &y);

        justClicked = false;
        
        if (x >= cRect->left() && x <=cRect->right() && y >= cRect->top() && y <= cRect->bottom()){
            color.r = std::min(base_color.r*1.5, (double)255);
            color.g = std::min(base_color.g*1.5, (double)255);
            color.b = std::min(base_color.b*1.5, (double)255);

            if (mouse & SDL_BUTTON(1)) {
                justClicked = true;
            }
        }
        else {
            color = base_color;
        }
            
    }

    void draw(SDL_Renderer* renderer) override {
        boxRGBA(renderer, cRect->left(), cRect->top(), cRect->right(), cRect->bottom(), 0x00, 0x00, 0x00, 0xFF);
        boxRGBA(renderer, cRect->left()+2, cRect->top()+2, cRect->right()-2, cRect->bottom()-2, color.r, color.g, color.b, color.a);

        Vector2<int> center = cRect->getAnchorPos(CDestRect::Anchor::CENTER_X | CDestRect::Anchor::CENTER_Y);
        fontTex->render(center.x, center.y, true);
    }
};