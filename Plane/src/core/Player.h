#ifndef PLAYER_H
#define PLAYER_H
#include "SDL2/SDL.h"

class Player {
public:
    Player(int windowWidth, int windowHeight);

    void moveLeft(int speed);
    void moveRight(int speed);
    void moveUp(int speed);
    void moveDown(int speed);

    int getX() const;
    int getY() const;

    void setX(int x);
    void setY(int y);

    SDL_Rect getCollisionRect() const;

private:
    int x;
    int y;
    int windowWidth;
    int windowHeight;
};
#endif // PLAYER_H
