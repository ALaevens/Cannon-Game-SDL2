#pragma once
#include "../Engine/ECS.hpp"
#include "../Engine/FontManager.hpp"

using namespace ECS;

struct CFontDisplay : Component {

    FontTexture* fontTex = nullptr;
    bool changed = true;
    std::string text = "";
    TextStyle style = {
        FontRenderStyle::SOLID,
        SDL_Color{0x00, 0x00, 0x00, 0xFF},
        SDL_Color{0x00, 0x00, 0x00, 0xFF}
    };
    int x = 0; int y = 0;

    CFontDisplay(TTF_Font* p_font, SDL_Renderer* renderer, std::string p_text, int p_x, int p_y) {
        fontTex = new FontTexture(p_font, renderer);
        setText(p_text);
        x = p_x, y = p_y;
    }

    // CFontDisplay(TTF_Font* p_font, SDL_Renderer* renderer) {
    //     fontTex = new FontTexture(p_font, renderer);
    // }

    ~CFontDisplay() {
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

    void update(double ms) override {
        if (changed) {
            fontTex->generate(text, style);
            changed = false;
        }
    }

    void draw(SDL_Renderer* renderer) override {
        fontTex->render(x, y);
    }

};