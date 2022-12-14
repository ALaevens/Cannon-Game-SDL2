#pragma once
#include "../Engine/ECS.hpp"
#include "Vector2.hpp"

using namespace ECS;

struct CDestRect : Component {
    enum Anchor : int {
        TOP         = 0x01,
        BOTTOM      = 0x02,
        LEFT        = 0x04,
        RIGHT       = 0x08,
        CENTER_X    = 0x10,
        CENTER_Y    = 0x20
    };

    SDL_Rect rect = {0, 0, 1, 1};
    int anchorFlag;
    
    CDestRect(int x, int y, int w, int h, int p_anchorFlag) {
        anchorFlag = p_anchorFlag;
        rect.w = w;
        rect.h = h;

        setPos(x, y);
    }

    CDestRect(int p_anchorFlag) : anchorFlag(p_anchorFlag) {}
    CDestRect() : CDestRect(TOP | LEFT) {}

    int top() { return rect.y; }
    int bottom() { return rect.y + rect.h; }
    int left() { return rect.x; }
    int right() {return rect.x + rect.w; }

    void setPos(int x, int y) {
        if (anchorFlag & TOP)
            rect.y = y;
        else if (anchorFlag & BOTTOM)
            rect.y = y - rect.h;
        else if (anchorFlag & CENTER_Y)
            rect.y = y - (rect.h / 2);

        if (anchorFlag & LEFT)
            rect.x = x;
        else if (anchorFlag & RIGHT)
            rect.x = x - rect.w;
        else if (anchorFlag & CENTER_X)
            rect.x = x - (rect.w / 2);

    }
    
    Vector2<int> middle() {
        Vector2<int> res{(rect.x + rect.w) / 2, (rect.y + rect.h) / 2};
        return res;
    }

    Vector2<int> getAnchorPos(int flags) {
        Vector2<int> res;

        if (anchorFlag & LEFT)
            res.x = rect.x;
        else if (anchorFlag & RIGHT)
            res.x =  rect.x + rect.w;
        else if (anchorFlag & CENTER_X)
            res.x = rect.x + (rect.w / 2);

        if (anchorFlag & TOP)
            res.y = rect.y;
        else if (anchorFlag & BOTTOM)
            res.y = rect.y + rect.h;
        else if (anchorFlag & CENTER_Y)
            res.y = rect.y + (rect.h / 2);

        return res;
    }

    Vector2<int> getAnchorPos() {
        return getAnchorPos(anchorFlag);
    }
};