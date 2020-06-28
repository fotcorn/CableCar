#include "Simulation.h"

#include <glm/glm.hpp>
#include <iostream>

#include "Components.h"
#include "Level.h"

constexpr float TIME_STEP = 1.0f / 60.0f;

constexpr float WORLD_TO_PHYSICS_RATIO = 20.0f;

Simulation::Simulation() : world(b2Vec2(0.0f, -10.0f)), worldBody(nullptr) {}

void Simulation::reset(entt::registry& reg) {
    // initialize world body
    b2BodyDef worldBodyDef;
    worldBodyDef.type = b2_staticBody;
    worldBodyDef.position.Set(0.0f, 0.0f);
    worldBody = world.CreateBody(&worldBodyDef);

    // iterate over Beams, create beams, create anchors
    reg.view<Beam, Sprite>().each([this, &reg](auto beamEntity, Beam& beam, Sprite& sprite) {
        // body
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(sprite.position.x / WORLD_TO_PHYSICS_RATIO, -sprite.position.y / WORLD_TO_PHYSICS_RATIO);
        bodyDef.angle = glm::radians(sprite.rotation);
        b2Body* beamBody = world.CreateBody(&bodyDef);
        reg.emplace<PhysicsBody>(beamEntity, beamBody);

        // fixture
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(sprite.dimensions.x / WORLD_TO_PHYSICS_RATIO, sprite.dimensions.y / WORLD_TO_PHYSICS_RATIO);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        beamBody->CreateFixture(&fixtureDef);

        // joints
        b2Body* startBody = getAnchorBody(reg, beam.start);
        b2Body* endBody = getAnchorBody(reg, beam.end);

        b2RevoluteJointDef revoluteJointDef;
        revoluteJointDef.collideConnected = false;

        revoluteJointDef.bodyA = startBody;
        revoluteJointDef.bodyB = beamBody;
        revoluteJointDef.localAnchorA.Set(0, 0);  // axis is at the center of the anchor
        revoluteJointDef.localAnchorB.Set(0, sprite.dimensions.x / 2 / WORLD_TO_PHYSICS_RATIO);
        world.CreateJoint(&revoluteJointDef);

        revoluteJointDef.bodyA = endBody;
        revoluteJointDef.bodyB = beamBody;
        revoluteJointDef.localAnchorA.Set(0, 0);  // axis is at the center of the anchor
        revoluteJointDef.localAnchorB.Set(0, -sprite.dimensions.x / 2 / WORLD_TO_PHYSICS_RATIO);
        world.CreateJoint(&revoluteJointDef);
    });
}

b2Body* Simulation::getAnchorBody(entt::registry& reg, entt::entity anchorEntity) {
    Anchor& anchor = reg.get<Anchor>(anchorEntity);
    if (anchor.levelAnchor) {
        // anchors from the level should not move, we therefore do not create
        // separate bodies for them, and just use the static world body for it
        return worldBody;
    } else {
        if (reg.has<PhysicsBody>(anchorEntity)) {
            return reg.get<PhysicsBody>(anchorEntity).body;
        } else {
            Sprite& sprite = reg.get<Sprite>(anchorEntity);
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(sprite.position.x / WORLD_TO_PHYSICS_RATIO,
                                 -sprite.position.y / WORLD_TO_PHYSICS_RATIO);
            b2Body* body = world.CreateBody(&bodyDef);

            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(1.0f, 1.0f);
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;
            body->CreateFixture(&fixtureDef);

            reg.emplace<PhysicsBody>(anchorEntity, body);
            return body;
        }
    }
}

void Simulation::tick(entt::registry& reg) {
    world.Step(TIME_STEP, 7, 7);

    reg.view<Sprite, PhysicsBody>().each([this](auto entity, Sprite& sprite, PhysicsBody& body) {
        std::ignore = entity;
        b2Vec2 position = body.body->GetPosition();
        float angle = body.body->GetAngle();
        sprite.position = glm::vec2(position.x * WORLD_TO_PHYSICS_RATIO, -position.y * WORLD_TO_PHYSICS_RATIO);
        sprite.rotation = glm::degrees(angle);
    });
}
