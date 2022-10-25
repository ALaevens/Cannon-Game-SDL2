#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <string>
#include <iostream>
#include <filesystem>

#include "Engine/util.hpp"

namespace fs = std::filesystem;

class SFXManager {
public:
    SFXManager(std::string root_dir_string);
    ~SFXManager();
    void prefetch_directory(std::string file_extension);
    Mix_Chunk* fetch_sfx(std::string path_segment);
    Mix_Music* fetch_music(std::string path_segment);

private:
    std::map<fs::path, Mix_Chunk*> loaded_effects;
    std::map<fs::path, Mix_Music*> loaded_songs;
    fs::path root_dir;

    Mix_Chunk* load_sfx_from_file(fs::path file_path);
    Mix_Music* load_music_from_file(fs::path file_path);
};