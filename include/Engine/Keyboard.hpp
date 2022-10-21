#pragma once

#include <SDL.h>
#include <set>

class Keyboard {
public:
    void updateKeys(SDL_Event* e) {
        if (e->type == SDL_KEYDOWN && pressed.count(e->key.keysym.sym) == 0) {
            // std::cout << "PRESS: " << SDL_GetKeyName(e->key.keysym.sym) << std::endl;
            just_pressed.insert(e->key.keysym.sym);
            pressed.insert(e->key.keysym.sym);
            
        } else if (e->type == SDL_KEYUP) {
            // std::cout << "RELEASE: " << SDL_GetKeyName(e->key.keysym.sym) << std::endl;
            if (pressed.count(e->key.keysym.sym) == 1) {
                pressed.extract(e->key.keysym.sym);
            }
        }
    }

    void refresh() {
        just_pressed.clear();
    }

    bool isKeyJustPressed(const SDL_Keycode key) {
        return just_pressed.count(key);
        return false;
    }

    bool isKeyPressed(const SDL_Keycode key) {
        return pressed.count(key);
        return false;
    }

private:
    std::set<SDL_Keycode> pressed;
    std::set<SDL_Keycode> just_pressed;
};