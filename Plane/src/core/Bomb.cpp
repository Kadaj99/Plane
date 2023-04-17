// Bomb.cpp
#include "Bomb.h"


Bomb::Bomb(int x, int y) : x(x), y(y), currentFrame(0), maxFrames(7), state(BombState::Inactive) {
}


Bomb::~Bomb() {
}

int Bomb::getX() const {
    return x;
}

int Bomb::getY() const {
    return y;
}

void Bomb::setX(int x) {
    this->x = x;
}

void Bomb::setY(int y) {
    this->y = y;
}

Bomb::BombState Bomb::getState() const {
    return state;
}

void Bomb::setState(BombState newState) {
    state = newState;
}


void Bomb::update(Uint32 currentTime) {
    if (state == BombState::Active) {
        if (currentTime - lastFrameTime >= frameDuration) {
            currentFrame++;
            lastFrameTime = currentTime;

            if (currentFrame >= 7) {
                state = BombState::Inactive;
            }
        }
    }
}

int Bomb::getCurrentFrame() const {
    return currentFrame;
}
