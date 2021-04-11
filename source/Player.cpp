#include "Player.h"
#include "resource_manager.h"

Player::Player(const std::string& name, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) : sprite(name,
                                                                                                           position,
                                                                                                           size, rotate,
                                                                                                           color) {

}

void Player::move(float value, movement key) {
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

void Player::moveAnimate(movement direction) {
    if (direction == LEFT) {
        texture = ResourceManager::GetTexture("leftPlayer/tile" + std::to_string(animationFrame));
    } else if (direction == RIGHT) {
        texture = ResourceManager::GetTexture("rightPlayer/tile" + std::to_string(animationFrame));
    }
    if (frameRate == 0) {
        frameRate++;
        animationFrame++;
        animationFrame %= 24;
    } else {
        frameRate++;
        frameRate %= 10;
    }
}

void Player::stop() {
    if (directionFacing == LEFT) {
        texture = ResourceManager::GetTexture("leftPlayerStill");
        animationFrame = 0;
    }
    else if (directionFacing == RIGHT) {
        texture = ResourceManager::GetTexture("rightPlayerStill");
        animationFrame = 0;
    }

}
