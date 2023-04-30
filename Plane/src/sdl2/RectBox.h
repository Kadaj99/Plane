#ifndef RECT_BOX_H
#define RECT_BOX_H

#include "SDL2/SDL.h"

class RectBox {
public:
    int x, y, w, h;

    RectBox(int x, int y, int w, int h);
    RectBox();

    bool operator==(const RectBox& other) const;
    bool operator!=(const RectBox& other) const;

    void set(int x, int y, int w, int h);

    bool hasIntersection(const RectBox &other) const;

    SDL_Rect toSDLRect() const;
};

#endif // RECT_BOX_H
