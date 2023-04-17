#include "Bullet.h"

Bullet::Bullet(int x, int y, int speed)
    : x(x), y(y), speed(speed), state(State::Inactive) {
}

void Bullet::update() {
    if (state == State::Active) {
        y -= speed;
    }
}

void Bullet::setX(int x) {
    this->x = x;
}

void Bullet::setY(int y) {
    this->y = y;
}

Bullet::State Bullet::getState() const {
    return state;
}

void Bullet::setState(State newState) {
    state = newState;
}

int Bullet::getX() const {
    return x;
}

int Bullet::getY() const {
    return y;
}

int Bullet::getSpeed() const {
    return speed;
}

SDL_Rect Bullet::getCollisionRect() const {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = 5; // 根据子弹图像的实际大小进行调整
    rect.h = 10; // 根据子弹图像的实际大小进行调整
    return rect;
}
