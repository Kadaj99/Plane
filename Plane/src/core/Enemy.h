#ifndef ENEMY_H
#define ENEMY_H

#include "SDL2/SDL.h"
#include "Bullet.h"
#include "Player.h"
#include "config.h"



class Enemy {
public:

     enum class EnemyState {
        Active,
        Inactive
    };

    Enemy(int x, int y, int interval, int speed);
    ~Enemy();

    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
    EnemyState getState() const;
    void setState(EnemyState newState);
    int getInterval() const;
    int getSpeed() const;

    void update();

    SDL_Rect getCollisionRect() const;

private:
    int x;
    int y;
    int interval;
    int speed;
    EnemyState state;
};

#endif
