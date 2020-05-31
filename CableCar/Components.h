#pragma once

#include <entt/entt.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <memory>

#include "Texture.h"

struct Sprite {
    glm::vec2 position;
    glm::vec2 dimensions;
    glm::vec2 origin;
    unsigned int layer;
    std::shared_ptr<Texture> texture;
};

struct CollisionCircle {
    glm::vec2 center;
    float radius;
    std::unique_ptr<float> collide(glm::vec2 coords) {
        std::unique_ptr<float> ret;
        float distance = glm::distance(center, coords);
        if (distance < radius) {
            ret = std::make_unique<float>(distance);
        }
        return ret;
    }
};

struct HoverTarget {
    std::shared_ptr<Texture> hoverTexture;
};

struct CurrentHover {};
