#include "TextureManager.hpp"

namespace fs = std::filesystem;

TextureManager::TextureManager(std::string root_dir_string, SDL_Renderer *renderer) {
    this->renderer = renderer;
    root_dir = root_dir_string;
    root_dir.make_preferred();

    std::cout << "Texture Root: " << root_dir.string() << std::endl; 
}

TextureManager::~TextureManager() {
    for (const auto& n : loaded_textures) {
        SDL_DestroyTexture(loaded_textures[n.first]);
    }
}

void TextureManager::prefetch_directory(std::string file_extension) {
    // recurse in root directory
    for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(root_dir)) {
        
        // add any files with the matching extension to the cache
        if (dir_entry.is_regular_file() && dir_entry.path().extension() == file_extension) {
            this->fetch(dir_entry.path().string().substr(root_dir.string().length() + 1));
        }
    }
}

SDL_Texture* TextureManager::fetch(std::string path_segment) {
    fs::path relative_path = {path_segment};
    relative_path.make_preferred();

    SDL_Texture* tex = nullptr;

    auto tex_pair = loaded_textures.find(relative_path);

    // texture not cached
    if (tex_pair == loaded_textures.end()) {
        std::cout << "Texture " << relative_path << " not found in cache. Loading...\n";

        fs::directory_entry full_path = fs::directory_entry(root_dir / relative_path);
        
        if (full_path.is_regular_file()) {
            std::cout << "LOAD: " << full_path << std::endl;
            tex = this->load_from_file(full_path.path());
            loaded_textures[relative_path] = tex;
        }


    } else {
        std::cout << "Texture " << relative_path << " found in cache\n";
        tex = tex_pair->second;
    }
    
    return tex;
}

SDL_Texture* TextureManager::load_from_file(fs::path file_path) {
    SDL_Surface* surf = IMG_Load(file_path.string().c_str());
    SDL_Texture* tex = nullptr;

    if (surf == nullptr) {
        show_sdl_error("Could not load image");
        return nullptr;
    }
    
    tex = SDL_CreateTextureFromSurface(renderer, surf);

    if (tex == nullptr) {
        show_sdl_error("Could not create texture");
        return nullptr;
    }  
    
    SDL_FreeSurface(surf);

    return tex;
}