#include "Map.h"
#include "config.h"

Map::Map()
{

    map1_posY = -GAME_HEIGHT;
    map2_posY = 0;

    scroll_speed = MAP_SCORLL_SPEED;

}

void Map::mapPosition(int scroll_speed) {
    map1_posY += scroll_speed;
    if (map1_posY >= GAME_HEIGHT) {
        map1_posY = -GAME_HEIGHT + scroll_speed;
    }

    map2_posY += scroll_speed;
    if (map2_posY >= GAME_HEIGHT) {
        map2_posY = -GAME_HEIGHT + scroll_speed;
    }
}


int Map::getMap1PosY() const {
    return map1_posY;
}

int Map::getMap2PosY() const {
    return map2_posY;
}
