#include "Drop.h"

Drop::Drop(int x, int y, int speed, DropType type)
: x(x), y(y), speed(speed), type(type), active(true)
{}

int Drop::getX() const {
    return x;
}

int Drop::getY() const {
    return y;
}

void Drop::setX(int setx) {
    x = setx;
}

void Drop::setY(int sety) {
    y = sety;
}

DropType Drop::getType() const {
    return type;
}

bool Drop::isActive() const {
    return active;
}

void Drop::setActive(bool active) {
    this->active = active;
}

void Drop::update() {
    // 更新补给位置
    y += speed;

    // 如果补给下落到窗口边界外，将其设置为非活动状态
    if (y > 768) {
        active = false;
    }
}

SDL_Rect Drop::getCollisionRect() const {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = 20; // 假设玩家宽度为 20
    rect.h = 20; // 假设玩家高度为 20
    return rect;
}