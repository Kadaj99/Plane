#ifndef BULLET_H
#define BULLET_H

#include "SDL2/SDL.h"

class Bullet {
public:

     enum class BulletState {
        Active,
        Inactive
    };

    Bullet(int x, int y, int speed);

    void setX(int x);
    void setY(int y);
    BulletState getState() const;
    void setState(BulletState newState);
    int getX() const;
    int getY() const;
    int getSpeed() const;

    void update();

    SDL_Rect getCollisionRect() const;

private:
    int x;
    int y;
    int speed;
    BulletState state;
};

#endif // BULLET_H
