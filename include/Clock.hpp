#ifndef CLOCK_H
#define CLOCK_H
#pragma once

#include <SDL.h>

class Clock {
public:
    Clock();
    double tick();

private:
    Uint32 last = 0;
    Uint32 current = 0;

    double duration = (current - last) / 1000.0; // last and current are in milliseconds
};


#endif