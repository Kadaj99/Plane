/**
 * @file Map.h
 * @brief Definition of the Map class.
 */

#ifndef MAP_H
#define MAP_H

#include "Bomb.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include <string>

/**
 * @class Map
 * @brief A class representing the game map.
 */
class Map {
public:
    /**
     * @brief Construct a new Map object.
     */
    Map();

    /**
     * @brief Update the position of the map.
     * @param scroll_speed The speed of scrolling.
     */
    void mapPosition(int scroll_speed);

    /**
     * @brief Get the width of the map.
     * @return The width of the map.
     */
    int getWidth() const;

    /**
     * @brief Get the height of the map.
     * @return The height of the map.
     */
    int getHeight() const;

    /**
     * @brief Get the position of map1.
     * @return The position of map1.
     */
    int getMap1PosY() const;

    /**
     * @brief Get the position of map2.
     * @return The position of map2.
     */
    int getMap2PosY() const;

private:
    int width_;                 /**< The width of the map. */
    int height_;                /**< The height of the map. */
    int map1_posY;              /**< The position of map1. */
    int map2_posY;              /**< The position of map2. */
    int scroll_speed;           /**< The speed of scrolling. */
};

#endif // MAP_H
