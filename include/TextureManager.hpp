#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <string>
#include <iostream>
#include <filesystem>

#include "util.hpp"

namespace fs = std::filesystem;

class TextureManager {
public:
    TextureManager(std::string root_dir_string, SDL_Renderer* renderer);
    ~TextureManager();
    void prefetch_directory(std::string file_extension);
    SDL_Texture* fetch(std::string path_segment);

private:
    std::map<fs::path, SDL_Texture*> loaded_textures;
    SDL_Renderer* renderer;
    fs::path root_dir;

    SDL_Texture* load_from_file(fs::path file_path);
};


#endif