/**
 * @file Player.h
 * @brief Definition of the Player class.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "SDL2/SDL.h"

/**
 * @class Player
 * @brief A class representing the game player.
 */
class Player {
public:
    /**
     * @brief Construct a new Player object.
     * @param windowWidth The width of the game window.
     * @param windowHeight The height of the game window.
     */
    Player(int windowWidth, int windowHeight);

    /**
     * @brief Move the player left.
     * @param speed The speed of movement.
     */
    void moveLeft(int speed);

    /**
     * @brief Move the player right.
     * @param speed The speed of movement.
     */
    void moveRight(int speed);

    /**
     * @brief Move the player up.
     * @param speed The speed of movement.
     */
    void moveUp(int speed);

    /**
     * @brief Move the player down.
     * @param speed The speed of movement.
     */
    void moveDown(int speed);

    /**
     * @brief Get the x-coordinate of the player.
     * @return The x-coordinate of the player.
     */
    int getX() const;

    /**
     * @brief Get the y-coordinate of the player.
     * @return The y-coordinate of the player.
     */
    int getY() const;

    /**
     * @brief Set the x-coordinate of the player.
     * @param x The new x-coordinate of the player.
     */
    void setX(int x);

    /**
     * @brief Set the y-coordinate of the player.
     * @param y The new y-coordinate of the player.
     */
    void setY(int y);
    
    /**
     * @brief Reset the player position to the center of the window.
     */
    void reset();

    /**
     * @brief Get the collision rectangle of the player.
     * @return The collision rectangle of the player.
     */
    SDL_Rect getCollisionRect() const;

     /**
     * @brief Get the player's health.
     * @return The player's health.
     */
    int getHealth() const;

    /**
     * @brief Set the player's health.
     * @param newHealth The new health value for the player.
     */
    void setHealth(int newHealth);

private:
    int x;                  /**< The x-coordinate of the player. */
    int y;                  /**< The y-coordinate of the player. */
    int windowWidth;        /**< The width of the game window. */
    int windowHeight;       /**< The height of the game window. */
    int health;             /**< The health of the player. */
};

#endif // PLAYER_H
