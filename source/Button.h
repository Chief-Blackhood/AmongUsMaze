//
// Created by pratyush on 29/03/21.
//

#ifndef HELLO_WORLD_BUTTON_H
#define HELLO_WORLD_BUTTON_H


#include "sprite.h"

class Button: public sprite {
    public:
    bool show=true;
    Button(const std::string& name, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);
};


#endif //HELLO_WORLD_BUTTON_H
