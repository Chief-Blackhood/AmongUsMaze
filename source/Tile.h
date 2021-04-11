//
// Created by pratyush on 26/03/21.
//

#ifndef HELLO_WORLD_TILE_H
#define HELLO_WORLD_TILE_H

#include <iostream>
#include "sprite.h"

class Tile:public sprite {
public:
    Tile(const std::string& name, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);
};


#endif //HELLO_WORLD_TILE_H
