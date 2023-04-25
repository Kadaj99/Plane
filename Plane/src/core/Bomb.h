/**
 * @file Bomb.h
 * @brief Definition of the Bomb class representing a bomb object.
 */

#ifndef BOMB_H
#define BOMB_H

#include "SDL2/SDL.h"

/**
 * @class Bomb
 * @brief A class representing a bomb object.
 */
class Bomb {
public:
    /**
     * @brief Construct a new Bomb object.
     * @param x The x-coordinate of the bomb.
     * @param y The y-coordinate of the bomb.
     */
    Bomb(int x, int y);

    /**
     * @brief Destroy the Bomb object.
     */
    ~Bomb();

    /**
     * @brief Enumeration of possible bomb states.
     */
    enum class BombState {
        Active,     /**< The bomb is active */
        Inactive    /**< The bomb is inactive */
    };

    /**
     * @brief Get the state of the bomb.
     * @return The state of the bomb.
     */
    BombState getState() const;

    /**
     * @brief Set the state of the bomb.
     * @param newState The new state.
     */
    void setState(BombState newState);

    /**
     * @brief Get the x-coordinate of the bomb.
     * @return The x-coordinate of the bomb.
     */
    int getX() const;

    /**
     * @brief Get the y-coordinate of the bomb.
     * @return The y-coordinate of the bomb.
     */
    int getY() const;

    /**
     * @brief Set the x-coordinate of the bomb.
     * @param x The new x-coordinate.
     */
    void setX(int x);

    /**
     * @brief Set the y-coordinate of the bomb.
     * @param y The new y-coordinate.
     */
    void setY(int y);

    /**
     * @brief Reset the bomb object.
     */
    void reset();

    /**
     * @brief Update the bomb object.
     * @param currentTime The current time.
     */
    void update(Uint32 currentTime);

    /**
     * @brief Get the current frame number.
     * @return The current frame number.
     */
    int getCurrentFrame() const;

private:
    int x;                      /**< The x-coordinate of the bomb. */
    int y;                      /**< The y-coordinate of the bomb. */
    int currentFrame;           /**< The current frame number. */
    int maxFrames;              /**< The maximum number of frames. */
    BombState state;            /**< The state of the bomb. */
    Uint32 lastFrameTime;       /**< The time of the last frame. */
    const Uint32 frameDuration = 60; /**< The duration of each frame. */
};

#endif // BOMB_H
