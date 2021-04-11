//
// Created by pratyush on 29/03/21.
//

#include "Imposter.h"
#include "resource_manager.h"

Imposter::Imposter(const std::string& name, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) : sprite(name,
                                                                                                                    position,
                                                                                                                    size, rotate,
                                                                                                                    color) {

}

void Imposter::move(float value, movement key) {
    switch (key) {
        case RIGHT: position = glm::vec2(position.x + value, position.y);
            break;
        case DOWN: position = glm::vec2(position.x, position.y + value);
            break;
        case LEFT: position = glm::vec2(position.x - value, position.y);
            break;
        case UP: position = glm::vec2(position.x, position.y - value);
            break;
        default: break;
    }
}

void Imposter::moveAnimate(movement direction) {
    if (direction == LEFT) {
        texture = ResourceManager::GetTexture("leftImposter/tile" + std::to_string(animationFrame));
    } else if (direction == RIGHT) {
        texture = ResourceManager::GetTexture("rightImposter/tile" + std::to_string(animationFrame));
    }
    if (frameRate == 0) {
        frameRate++;
        animationFrame++;
        animationFrame %= 12;
    } else {
        frameRate++;
        frameRate %= 30;
    }
}