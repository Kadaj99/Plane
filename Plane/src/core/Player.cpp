#include "Player.h"
#include "config.h"

Player::Player(int windowWidth, int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight) {
    x = windowWidth / 2;
    y = windowHeight - 50; // 假设玩家对象的高度约为 50
}

void Player::moveLeft(int speed) {
    x -= speed;
    if (x < 0) {
        x = 0;
    }
}

void Player::moveRight(int speed) {
    x += speed;
    if (x > windowWidth - 50) { // 假设玩家对象的宽度约为 50
        x = windowWidth - 50;
    }
}

void Player::moveUp(int speed) {
    y -= speed;
    if (y < 0) {
        y = 0;
    }
}

void Player::moveDown(int speed) {
    y += speed;
    if (y > windowHeight - 50) { // 假设玩家对象的高度约为 50
        y = windowHeight - 50;
    }
}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}

void Player::setX(int x) {
    this->x = x;
    if (this->x < 0) {
        this->x = 0;
    } else if (this->x > windowWidth - 50) {
        this->x = windowWidth - 50;
    }
}

void Player::setY(int y) {
    this->y = y;
    if (this->y < 0) {
        this->y = 0;
    } else if (this->y > windowHeight - 50) {
        this->y = windowHeight - 50;
    }
}


SDL_Rect Player::getCollisionRect() const {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = 50; // 假设玩家宽度为 50
    rect.h = 50; // 假设玩家高度为 50
    return rect;
}
