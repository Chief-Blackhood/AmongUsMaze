

#include <utility>
#include "sprite.h"
#include "resource_manager.h"

void sprite::draw() {

}

sprite::sprite(const std::string& name, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
: position(position), size(size), rotate(rotate), color(color) {
    texture = ResourceManager::GetTexture(name);
}