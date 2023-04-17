#include "Enemy.h"

Enemy::Enemy(int x, int y, int interval, int speed)
: x(x), y(y), interval(interval), speed(speed), state(State::Active)
{}

Enemy::~Enemy()
{}

int Enemy::getX() const {
    return x;
}

int Enemy::getY() const {
    return y;
}

void Enemy::setX(int x) {
    x = x;
}

void Enemy::setY(int y) {
    y = y;
}

State Enemy::getState() const {
    return state;
}

void Enemy::setState(State newState) {
    state = newState;
}

int Enemy::getInterval() const {
    return interval;
}

int Enemy::getSpeed() const {
    return speed;
}

void Enemy::update() {
}

SDL_Rect Enemy::getCollisionRect() const {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = 20; // 假设玩家宽度为 20
    rect.h = 20; // 假设玩家高度为 20
    return rect;
}
