//
// Created by pratyush on 29/03/21.
//

#ifndef HELLO_WORLD_IMPOSTER_H
#define HELLO_WORLD_IMPOSTER_H

#include <iostream>
#include "sprite.h"
class Imposter:public sprite {
    public:
    bool dead=false;
    movement direction;
    int animationFrame = 0;
    movement directionFacing = RIGHT;
    int frameRate = 5;
    Imposter(const std::string& name, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);

    void move(float value, movement key);
    void moveAnimate(movement direction);
};


#endif //HELLO_WORLD_IMPOSTER_H
