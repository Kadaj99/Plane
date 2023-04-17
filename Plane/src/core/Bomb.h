// Bomb.h
#ifndef BOMB_H
#define BOMB_H

#include "SDL2/SDL.h"

class Bomb {
public:
    enum class BombState {
        Active,
        Inactive
    };

    Bomb(int x, int y);
    ~Bomb();

    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
    BombState getState() const;
    void setState(BombState newState);

    void update(Uint32 currentTime);
    int getCurrentFrame() const;


private:
    int x;
    int y;
    int currentFrame;
    int maxFrames;
    BombState state;

    Uint32 lastFrameTime;
    const Uint32 frameDuration = 60;
};

#endif
