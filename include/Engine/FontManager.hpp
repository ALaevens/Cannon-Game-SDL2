#ifndef FONTMANAGER_H
#define FONTMANAGER_H
#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

typedef struct FontDescriptor {
    fs::path path;
    int size;

    bool operator==(const FontDescriptor &r) const {
        return path == r.path && size == r.size;
    }

    bool operator<(const FontDescriptor &r) const {
        return path < r.path || (path == r.path && size < r.size);
    }
} FontDescriptor;

class FontManager {
public:
    FontManager(std::string root_dir_string, SDL_Renderer* renderer);
    ~FontManager();

    TTF_Font* fetchFont(std::string path_segment, int size);

private:
    std::map<FontDescriptor, TTF_Font*> loaded_fonts;
    SDL_Renderer* renderer;
    fs::path root_dir;

    TTF_Font* load_from_file(fs::path full_path, int size);
};

typedef enum FontRenderStyle {
    SOLID,
    SHADED,
    BLENDED
} FontRenderStyle;

typedef struct TextStyle {
    FontRenderStyle renderStyle;
    SDL_Color fg;
    SDL_Color bg;
} TextStyle;

class FontTexture {
public:
    FontTexture(TTF_Font* p_font, SDL_Renderer* p_renderer) : font(p_font), renderer(p_renderer) {};
    ~FontTexture();
    void generate(std::string text, TextStyle style);
    void render(int x, int y);
    void render(int x, int y, bool center);

    bool isGenerated() { return tex != nullptr; }

private:
    TTF_Font* font = nullptr;
    SDL_Texture* tex = nullptr;
    SDL_Renderer* renderer;
    SDL_Rect dest;
};

#endif