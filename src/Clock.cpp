#include "Clock.hpp"

Clock::Clock() {
    current = SDL_GetTicks();
    last = current;
}

double Clock::tick() {
    last = current;

    current = SDL_GetTicks();

    double duration = (current - last) / 1000.0;

    return duration;
}