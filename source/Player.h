//
// Created by pratyush on 26/03/21.
//

#ifndef HELLO_WORLD_PLAYER_H
#define HELLO_WORLD_PLAYER_H

#include <iostream>
#include "sprite.h"

class Player: public sprite {
public:
    int animationFrame = 0;
    movement directionFacing = RIGHT;
    int frameRate = 5;
    Player(const std::string& name, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);
    void move(float , movement key);
    void moveAnimate(movement direction);

    void stop();
};


#endif //HELLO_WORLD_PLAYER_H
