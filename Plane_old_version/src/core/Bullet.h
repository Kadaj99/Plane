/**
 * @file Bullet.h
 * @brief Definition of the Bullet class representing a bullet object.
 */

#ifndef BULLET_H
#define BULLET_H

#include "SDL2/SDL.h"

/**
 * @class Bullet
 * @brief A class representing a bullet object.
 */
class Bullet {
public:
    enum class BulletTrajectory {
            Weapon1,
            Weapon2,
            Weapon3,
            Weapon4,
            Weapon5
    };

    BulletTrajectory trajectory;
    /**
     * @brief Construct a new Bullet object.
     * @param x The x-coordinate of the bullet.
     * @param y The y-coordinate of the bullet.
     * @param speed The speed of the bullet.
     */
    
    Bullet(int x, int y, int speed, BulletTrajectory trajectory);


    /**
     * @brief Enumeration of possible bullet states.
     */
    enum class BulletState {
        Active,     /**< The bullet is active */
        Inactive    /**< The bullet is inactive */
    };

    /**
     * @brief Set the x-coordinate of the bullet.
     * @param x The new x-coordinate.
     */
    void setX(int x);

    /**
     * @brief Set the y-coordinate of the bullet.
     * @param y The new y-coordinate.
     */
    void setY(int y);

    /**
     * @brief Get the state of the bullet.
     * @return The state of the bullet.
     */
    BulletState getState() const;

    /**
     * @brief Set the state of the bullet.
     * @param newState The new state.
     */
    void setState(BulletState newState);

    /**
     * @brief Get the x-coordinate of the bullet.
     * @return The x-coordinate of the bullet.
     */
    int getX() const;

    /**
     * @brief Get the y-coordinate of the bullet.
     * @return The y-coordinate of the bullet.
     */
    int getY() const;

    /**
     * @brief Get the speed of the bullet.
     * @return The speed of the bullet.
     */
    int getSpeed() const;

    /**
     * @brief Set the speed of the bullet.
     * @param speed The new speed.
     */
    void setSpeed(int speed);

    /**
     * @brief Reset the bullet object.
     */
    void reset();

     /**
     * @brief Get the bullet's damage.
     * @return The bullet's damage.
     */
    int getDamage() const;

    /**
     * @brief Set the bullet's damage.
     * @param newDamage The new damage value for the bullet.
     */
    void setDamage(int newDamage);

    /**
     * @brief Get the bullet's fire interval.
     * @return The bullet's fire interval.
     */
    int getFireInterval() const;

    /**
     * @brief Set the bullet's fire interval.
     * @param newFireInterval The new fire interval for the bullet.
     */
    void setFireInterval(int newFireInterval);

    /**
     * @brief Get the bullet's trajectory.
     * @return The bullet's trajectory.
     */
    int getTrajectory() const;

    /**
     * @brief Set the bullet's trajectory.
     * @param newTrajectory The new trajectory for the bullet.
     */
    void setTrajectory(BulletTrajectory newTrajectory);


    /**
     * @brief Update the bullet object.
     */
    void update();

    void setSpeedX(float speedX);
    void setSpeedY(float speedY);



    /**
     * @brief Get the collision rectangle of the bullet.
     * @return The collision rectangle of the bullet.
     */
    SDL_Rect getCollisionRect() const;


private:
    int x;                      /**< The x-coordinate of the bullet. */
    int y;                      /**< The y-coordinate of the bullet. */
    float speedX;
    float speedY;
    int speed;                  /**< The speed of the bullet. */
    BulletState state;          /**< The state of the bullet. */
    int damage; // 子弹伤害
    int fireInterval; // 子弹生成间隔（攻击速度）
    
};
    
#endif // BULLET_H
