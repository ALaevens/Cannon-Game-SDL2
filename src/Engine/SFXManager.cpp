#include "Engine/SFXManager.hpp"

namespace fs = std::filesystem;

SFXManager::SFXManager(std::string root_dir_string) {
    root_dir = root_dir_string;
    root_dir.make_preferred();
    std::stringstream ss;
    ss << "Sound Root: " << root_dir.string() << std::endl;
    log_out(ss);
}

SFXManager::~SFXManager() {
    for (const auto& n : loaded_effects) {
        Mix_FreeChunk(loaded_effects[n.first]);
    }

    for (const auto& n : loaded_songs) {
        Mix_FreeMusic(loaded_songs[n.first]);
    }
}

void SFXManager::prefetch_directory(std::string file_extension) {
    // recurse in root directory
    for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(root_dir)) {
        
        // add any files with the matching extension to the cache
        if (dir_entry.is_regular_file() && dir_entry.path().extension() == file_extension) {
            this->fetch_sfx(dir_entry.path().string().substr(root_dir.string().length() + 1));
        }
    }
}

Mix_Chunk* SFXManager::fetch_sfx(std::string path_segment) {
    fs::path relative_path = {path_segment};
    relative_path.make_preferred();    

    Mix_Chunk* chunk = nullptr;
    auto sfx_pair = loaded_effects.find(relative_path);

    // sfx not cached
    if (sfx_pair == loaded_effects.end()) {
        std::stringstream ss;
        ss << "SFX " << relative_path << " not found in cache. Loading...\n";
        log_out(ss);

        fs::directory_entry full_path = fs::directory_entry(root_dir / relative_path);
        
        // load and store file if exists
        if (full_path.is_regular_file()) {
            std::stringstream ss;
            ss << "LOAD: " << full_path << std::endl;
            log_out(ss);
            
            chunk = this->load_sfx_from_file(full_path.path());
            loaded_effects[relative_path] = chunk;
        }

    } else {
        chunk = sfx_pair->second;
    }

    return chunk;
}

Mix_Chunk* SFXManager::load_sfx_from_file(fs::path file_path) {
    Mix_Chunk* chunk = Mix_LoadWAV(file_path.string().c_str());

    if (chunk == nullptr) {
        show_sdl_error("Could not load sound effect");
        return nullptr;
    }

    return chunk;
}

Mix_Music* SFXManager::fetch_music(std::string path_segment) {
    fs::path relative_path = {path_segment};
    relative_path.make_preferred();    

    Mix_Music* song = nullptr;
    auto song_pair = loaded_songs.find(relative_path);

    // sfx not cached
    if (song_pair == loaded_songs.end()) {
        std::stringstream ss;
        ss << "Song " << relative_path << " not found in cache. Loading...\n";
        log_out(ss);

        fs::directory_entry full_path = fs::directory_entry(root_dir / relative_path);
        
        // load and store file if exists
        if (full_path.is_regular_file()) {
            std::stringstream ss;
            ss << "LOAD: " << full_path << std::endl;
            log_out(ss);
            
            song = this->load_music_from_file(full_path.path());
            loaded_songs[relative_path] = song;
        }

    } else {
        song = song_pair->second;
    }

    return song;
}

Mix_Music* SFXManager::load_music_from_file(fs::path file_path) {
    Mix_Music* song = Mix_LoadMUS(file_path.string().c_str());

    if (song == nullptr) {
        show_sdl_error("Could not load song");
        return nullptr;
    }

    return song;
}