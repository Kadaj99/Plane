#pragma once

#include <SDL2/SDL.h>

enum class DropType {
    IncreaseAttackPower,
    HealthRecovery,
    DecreaseFireInterval
};

class Drop {
private:
    int x, y;
    int speed;
    DropType type;
    Uint32 spawnTime;
    bool active;
    

public:
    Drop(int x, int y, int speed, DropType type);

    int getX() const;
    int getY() const;
    void setX(int setx);
    void setY(int sety);
    DropType getType() const;
    bool isActive() const;
    void setActive(bool active);

    void update();
    SDL_Rect getCollisionRect() const;
};
