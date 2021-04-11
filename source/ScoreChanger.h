//
// Created by pratyush on 29/03/21.
//

#ifndef HELLO_WORLD_SCORECHANGER_H
#define HELLO_WORLD_SCORECHANGER_H


#include "sprite.h"

class ScoreChanger: public sprite{
public:
    ScoreChanger(const std::string& name, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);
};


#endif //HELLO_WORLD_SCORECHANGER_H
