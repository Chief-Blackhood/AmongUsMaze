//
// Created by pratyush on 31/03/21.
//

#ifndef HELLO_WORLD_TEXT_H
#define HELLO_WORLD_TEXT_H


#include "sprite.h"

class Text: public sprite {
public:
    Text(const std::string& name, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);
};


#endif //HELLO_WORLD_TEXT_H
