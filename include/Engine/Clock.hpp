#ifndef CLOCK_H
#define CLOCK_H
#pragma once

#include <SDL.h>

class Clock {
public:
    Clock(Uint64 p_minDuration);
    Clock();
    Uint64 tick();

private:
    Uint64 last;
    Uint64 current;
    Uint64 duration;
    Uint64 minDuration;
};


#endif