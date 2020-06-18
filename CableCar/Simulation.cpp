#include "Simulation.h"

#include <iostream>

#include "Components.h"
#include "Level.h"

constexpr float TIME_STEP = 1.0f / 60.0f;

constexpr float WORLD_TO_PHYSICS_RATIO = 20.0f;

Simulation::Simulation() : world(b2Vec2(0.0f, -10.0f)) {}

void Simulation::reset(entt::registry& reg) {
    entt::entity entity = Level::createAnchor(reg, 100.0f, 100.0f, false);

    Sprite& sprite = reg.get<Sprite>(entity);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(sprite.position.x / WORLD_TO_PHYSICS_RATIO, -sprite.position.y / WORLD_TO_PHYSICS_RATIO);
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);

    reg.emplace<PhysicsBody>(entity, body);
}

void Simulation::tick(entt::registry& reg) {
    world.Step(TIME_STEP, 7, 7);

    reg.view<Sprite, PhysicsBody>().each([this](auto entity, Sprite& sprite, PhysicsBody& body) {
        std::ignore = entity;
        b2Vec2 position = body.body->GetPosition();
        std::cout << position.x << " " << position.y << std::endl;
        sprite.position = glm::vec2(position.x * WORLD_TO_PHYSICS_RATIO, -position.y * WORLD_TO_PHYSICS_RATIO);
    });
}
