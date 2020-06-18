#pragma once

#include <entt/entt.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include <Box2D/Box2D.h>

#include "Texture.h"

struct Sprite {
    glm::vec2 position = glm::vec2(0.0f, 0.0f);
    glm::vec2 dimensions = glm::vec2(0.0f, 0.0f);
    glm::vec2 origin = glm::vec2(0.0f, 0.0f);
    float rotation = 0.0f;
    unsigned int layer = 0;
    std::shared_ptr<Texture> texture;
};

struct CollisionCircle {
    glm::vec2 center = glm::vec2(0.0f, 0.0f);
    float radius = 0.0f;
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

// game elements
struct Anchor {
    // anchor is part of the level and not placed by the player
    // * will not be saved into a savegame
    // * does not get removed when last beam connected to it is removed
    bool levelAnchor;
};

struct Beam {
    entt::entity start;
    entt::entity end;
};

struct PhysicsBody {
    b2Body* body;
};
