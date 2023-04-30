#include "Bullet.h"


Bullet::Bullet(int x, int y, int speed, BulletTrajectory trajectory)
    : x(x), y(y), speed(speed), trajectory(trajectory), state(BulletState::Active) {}



void Bullet::update() {
    if (state == BulletState::Active) {
        x += speedX;
        y += speedY;

        // 如果子弹超出窗口边界，将状态设置为"Inactive"
        if (y < 0 || x < 0 || x > 512) {
            state = BulletState::Inactive;
        }
    }
}

void Bullet::setX(int x) {
    this->x = x;
}

void Bullet::setY(int y) {
    this->y = y;
}

Bullet::BulletState Bullet::getState() const {
    return state;
}

void Bullet::setState(BulletState newState) {
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

void Bullet::setSpeed(int speed) {
    this->speed = speed;
}

void Bullet::setSpeedX(float speedX) {
    this->speedX = speedX;
}

void Bullet::setSpeedY(float speedY) {
    this->speedY = speedY;
}

void Bullet::reset(){
    state=BulletState::Inactive;
}

int Bullet::getDamage() const {
    return damage;
}

void Bullet::setDamage(int newDamage) {
    damage = newDamage;
}

int Bullet::getFireInterval() const {
    return fireInterval;
}

void Bullet::setFireInterval(int newFireInterval) {
    fireInterval = newFireInterval;
}

void Bullet::setTrajectory(BulletTrajectory newTrajectory) {
    trajectory = newTrajectory;
}

SDL_Rect Bullet::getCollisionRect() const {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = 10; // 根据子弹图像的实际大小进行调整
    rect.h = 20; // 根据子弹图像的实际大小进行调整
    return rect;
}
