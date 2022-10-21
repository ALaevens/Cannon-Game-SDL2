#include "Engine/SFXManager.hpp"

namespace fs = std::filesystem;

SFXManager::SFXManager(std::string root_dir_string) {
    root_dir = root_dir_string;
    root_dir.make_preferred();
    std::cout << "Sound Root: " << root_dir.string() << std::endl; 
}

SFXManager::~SFXManager() {
    for (const auto& n : loaded_effects) {
        Mix_FreeChunk(loaded_effects[n.first]);
    }
}

// void TextureManager::prefetch_directory(std::string file_extension) {
//     // recurse in root directory
//     for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(root_dir)) {
        
//         // add any files with the matching extension to the cache
//         if (dir_entry.is_regular_file() && dir_entry.path().extension() == file_extension) {
//             this->fetch(dir_entry.path().string().substr(root_dir.string().length() + 1));
//         }
//     }
// }

void SFXManager::prefetch_directory(std::string file_extension) {
    // recurse in root directory
    for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(root_dir)) {
        
        // add any files with the matching extension to the cache
        if (dir_entry.is_regular_file() && dir_entry.path().extension() == file_extension) {
            this->fetch_sfx(dir_entry.path().string().substr(root_dir.string().length() + 1));
        }
    }
}

// SDL_Texture* TextureManager::fetch(std::string path_segment) {
//     fs::path relative_path = {path_segment};
//     relative_path.make_preferred();

//     SDL_Texture* tex = nullptr;

//     auto tex_pair = loaded_textures.find(relative_path);

//     // texture not cached
//     if (tex_pair == loaded_textures.end()) {
//         std::cout << "Texture " << relative_path << " not found in cache. Loading...\n";

//         fs::directory_entry full_path = fs::directory_entry(root_dir / relative_path);
        
//         // load and store file (if it exists)
//         if (full_path.is_regular_file()) {
//             std::cout << "LOAD: " << full_path << std::endl;
//             tex = this->load_from_file(full_path.path());
//             loaded_textures[relative_path] = tex;
//         }

//     } else {
//         tex = tex_pair->second;
//     }
    
//     return tex;
// }

Mix_Chunk* SFXManager::fetch_sfx(std::string path_segment) {
    fs::path relative_path = {path_segment};
    relative_path.make_preferred();    

    Mix_Chunk* chunk = nullptr;
    auto sfx_pair = loaded_effects.find(relative_path);

    // sfx not cached
    if (sfx_pair == loaded_effects.end()) {
        std::cout << "SFX " << relative_path << " not found in cache. Loading...\n";

        fs::directory_entry full_path = fs::directory_entry(root_dir / relative_path);
        
        // load and store file if exists
        if (full_path.is_regular_file()) {
            std::cout << "LOAD: " << full_path << std::endl;
            chunk = this->load_from_file(full_path.path());
            loaded_effects[relative_path] = chunk;
        }

    } else {
        chunk = sfx_pair->second;
    }

    return chunk;
}

// SDL_Texture* TextureManager::load_from_file(fs::path file_path) {
//     SDL_Surface* surf = IMG_Load(file_path.string().c_str());
//     SDL_Texture* tex = nullptr;

//     if (surf == nullptr) {
//         show_sdl_error("Could not load image");
//         return nullptr;
//     }
    
//     tex = SDL_CreateTextureFromSurface(renderer, surf);

//     if (tex == nullptr) {
//         show_sdl_error("Could not create texture");
//         return nullptr;
//     }  
    
//     SDL_FreeSurface(surf);

//     return tex;
// }

Mix_Chunk* SFXManager::load_from_file(fs::path file_path) {
    Mix_Chunk* chunk = Mix_LoadWAV(file_path.string().c_str());

    if (chunk == nullptr) {
        show_sdl_error("Could not load sound effect");
        return nullptr;
    }

    return chunk;
}