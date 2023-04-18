#ifndef MAP_H
#define MAP_H
#include "Bomb.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include "config.h"
#include <string>

const int MAX_MAP_WIDTH = 512;
const int MAX_MAP_HEIGHT = 768;

class Map {
public:
    // 构造函数和析构函数
    Map();

    void mapPosition(int scroll_speed);


    // 获取地图的宽度和高度
    int getWidth() const;
    int getHeight() const;

    int map1_posY;
    int map2_posY;

    int scroll_speed;

    int getMap1PosY() const; // 添加 getMap1PosY 方法声明
    int getMap2PosY() const; // 添加 getMap2PosY 方法声明

private:
    int width_;                 // 地图宽度
    int height_;                // 地图高度

};

#endif
