#ifndef BULLET_H
#define BULLET_H

#include "SDL2/SDL.h"

class Bullet {
public:

    enum class State {
        Active,
        Inactive
    };
    Bullet(int x, int y, int speed);

    void setX(int x);
    void setY(int y);
    State getState() const;
    void setState(State newState);
    int getX() const;
    int getY() const;
    int getSpeed() const;

    void update();

    SDL_Rect getCollisionRect() const;

private:
    int x;
    int y;
    int speed;
    State state;
};

#endif // BULLET_H
