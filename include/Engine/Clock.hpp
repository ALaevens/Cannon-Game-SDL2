#ifndef CLOCK_H
#define CLOCK_H
#pragma once

#include <SDL.h>

class Clock {
public:
    Clock(double p_minDuration);
    Clock();
    double tick();

private:
    Uint64 last;
    Uint64 current;
    Uint64 tps;
    double duration;
    double minDuration;
};


#endif