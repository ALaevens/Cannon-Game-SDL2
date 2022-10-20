#include "Engine/Clock.hpp"
#include <iostream>

Clock::Clock() : Clock(0u) {}

Clock::Clock(Uint64 p_minDuration) : minDuration(p_minDuration) {
    current = SDL_GetTicks();
    last = current;
}

Uint64 Clock::tick() {
    last = current;
    current = SDL_GetTicks64();
    Uint64 duration = (current - last);

    if (duration < minDuration && minDuration > 0) {
        Uint64 diff = minDuration - duration;
        SDL_Delay(diff);
        duration += diff;
    }

    return duration;
}