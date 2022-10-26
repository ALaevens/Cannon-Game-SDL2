#include "Engine/Clock.hpp"
#include "Engine/util.hpp"
#include <iostream>

Clock::Clock() : Clock(-1.0) {}

Clock::Clock(double p_minDuration) : minDuration(p_minDuration) {
    tps = SDL_GetPerformanceFrequency();

    std::stringstream ss;
    ss << "Clock using a resolution of " << tps << " ticks / sec\n";
    log_out(ss);
    
    current = SDL_GetPerformanceCounter();
    last = current;
}

double Clock::tick() {
    last = current;

    double now = SDL_GetPerformanceCounter();
    double duration = (double)((now - last)*1000) / tps;

    if (duration < minDuration && minDuration > 0) {
        int diff = ceil(minDuration - duration);
        SDL_Delay(diff);

        double now = SDL_GetPerformanceCounter();
        duration = (double)((now - last)*1000) / tps; 
    }

    current = now;
    return duration;
}