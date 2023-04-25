/**
 * @file Enemy.h
 * @brief Definition of the Enemy class representing an enemy object.
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "SDL2/SDL.h"
#include "Bullet.h"
#include "Player.h"


/**
 * @class Enemy
 * @brief A class representing an enemy object.
 */
class Enemy {
public:
    /**
     * @brief Construct a new Enemy object.
     * @param x The x-coordinate of the enemy.
     * @param y The y-coordinate of the enemy.
     * @param interval The time interval between each enemy bullet shot.
     * @param speed The speed of the enemy's movement.
     */
    Enemy(int x, int y, int interval, int speed);

    /**
     * @brief Enumeration of possible enemy states.
     */
    enum class EnemyState {
        Active,     /**< The enemy is active */
        Inactive    /**< The enemy is inactive */
    };

    /**
     * @brief Get the x-coordinate of the enemy.
     * @return The x-coordinate of the enemy.
     */
    int getX() const;

    /**
     * @brief Get the y-coordinate of the enemy.
     * @return The y-coordinate of the enemy.
     */
    int getY() const;

    /**
     * @brief Set the x-coordinate of the enemy.
     * @param x The new x-coordinate.
     */
    void setX(int x);

    /**
     * @brief Set the y-coordinate of the enemy.
     * @param y The new y-coordinate.
     */
    void setY(int y);

    /**
     * @brief Get the state of the enemy.
     * @return The state of the enemy.
     */
    EnemyState getState() const;

    /**
     * @brief Set the state of the enemy.
     * @param newState The new state.
     */
    void setState(EnemyState newState);

    /**
     * @brief Get the time interval between each enemy bullet shot.
     * @return The time interval.
     */
    int getInterval() const;

    /**
     * @brief Get the speed of the enemy's movement.
     * @return The speed of the enemy's movement.
     */
    int getSpeed() const;

    /**
     * @brief Set the speed of the enemy's movement.
     * @param setSpeed The new speed.
     */
    void setSpeed(int setSpeed);

    /**
     * @brief Update the enemy object.
     */
    void update();

    /**
     * @brief Reset the enemy object.
     */
    void reset();

    /**
     * @brief Get the collision rectangle of the enemy.
     * @return The collision rectangle of the enemy.
     */
    SDL_Rect getCollisionRect() const;

private:
    int x;                      /**< The x-coordinate of the enemy. */
    int y;                      /**< The y-coordinate of the enemy. */
    int interval;               /**< The time interval between each enemy bullet shot. */
    int speed;                  /**< The speed of the enemy's movement. */
    EnemyState state;           /**< The state of the enemy. */
};

#endif // ENEMY_H
