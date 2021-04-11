//
// Created by pratyush on 26/03/21.
//

#ifndef HELLO_WORLD_SPRITE_H
#define HELLO_WORLD_SPRITE_H

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
enum movement { RIGHT, LEFT, UP, DOWN };
class sprite {
public:
    Texture2D texture;
    glm::vec2 position;
    glm::vec2 size = glm::vec2(10.0f, 10.0f);
    float rotate = 0.0f;
    glm::vec3 color = glm::vec3(1.0f);


    void draw();
    sprite(const std::string& name, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);
};


#endif //HELLO_WORLD_SPRITE_H
