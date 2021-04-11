//
// Created by pratyush on 26/03/21.
//

#include "Tile.h"

Tile::Tile(const std::string& name, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) : sprite(name, position,
                                                                                                         size, rotate,
                                                                                                         color) {

}
