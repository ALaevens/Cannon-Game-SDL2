#include "Engine/FontManager.hpp"
#include "Engine/util.hpp"
#include <iostream>

FontManager::FontManager(std::string root_dir_string, SDL_Renderer* renderer) {
    this->renderer = renderer;
    root_dir = root_dir_string;
    root_dir.make_preferred();

    std::stringstream ss;
    ss << "Font Root: " << root_dir.string() << std::endl; 
    log_out(ss);
}

FontManager::~FontManager() {
    for (const auto& n : loaded_fonts) {
        TTF_CloseFont(loaded_fonts[n.first]);
    }
}

TTF_Font* FontManager::fetchFont(std::string path_segment, int size) {
    fs::path relative_path = {path_segment};
    relative_path.make_preferred();

    TTF_Font* font = nullptr;

    FontDescriptor identifier = {relative_path, size};
    auto font_pair = loaded_fonts.find(identifier);

    // font not cached
    if (font_pair == loaded_fonts.end()) {
        std::stringstream ss;
        ss << "Font " << relative_path << " (" << size << " pt) not found in cache. Loading...\n";
        log_out(ss);

        fs::directory_entry full_path = fs::directory_entry(root_dir / relative_path);
        
        // load and store file (if it exists)
        if (full_path.is_regular_file()) {
            std::stringstream ss;
            ss << "LOAD: " << full_path << std::endl;
            log_out(ss);

            font = this->load_from_file(full_path, identifier.size);
            loaded_fonts[identifier] = font;
        }

    } else {
        font = font_pair->second;
    }
    
    if (font == nullptr) {
        std::stringstream ss;
        ss << "WARNING: NULL FONT\n";
        log_out(ss);
    }

    return font;
}

TTF_Font* FontManager::load_from_file(fs::path full_path, int size) {
    TTF_Font* font = TTF_OpenFont(full_path.string().c_str(), size);
    if (font == nullptr) {
        show_sdl_error("Couldn't open font file");
    }
    
    return font;
}

FontTexture::~FontTexture() {
    if (tex)
        SDL_DestroyTexture(tex);
}

void FontTexture::generate(std::string text, TextStyle style) {
    if (tex)
        SDL_DestroyTexture(tex);

    SDL_Surface* surf;

    switch (style.renderStyle) {
    case FontRenderStyle::SOLID:
        surf = TTF_RenderUTF8_Solid(font, text.c_str(), style.fg);
        break;
    case FontRenderStyle::SHADED:
        surf = TTF_RenderUTF8_Shaded(font, text.c_str(), style.fg, style.bg);
        break;
    case FontRenderStyle::BLENDED:
        surf = TTF_RenderUTF8_Blended(font, text.c_str(), style.fg);
        break;
    default:
        surf = TTF_RenderUTF8_Solid(font, text.c_str(), style.fg);
        break;
    }

    dest.w = surf->w;
    dest.h = surf->h;

    tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void FontTexture::render(int x, int y) {
    dest.x = x; dest.y = y;

    SDL_RenderCopy(renderer, tex, nullptr, &dest); 
}