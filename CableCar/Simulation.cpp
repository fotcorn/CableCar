#include "Simulation.h"

#include <glm/glm.hpp>
#include <iostream>

#include "Components.h"
#include "GlobalConstants.h"
#include "Level.h"

constexpr float TIME_STEP = 1.0f / 60.0f;

constexpr float WORLD_TO_PHYSICS_RATIO = 20.0f;

Simulation::Simulation() : m_world(b2Vec2(0.0f, -10.0f)), m_debugDraw(WORLD_TO_PHYSICS_RATIO) {
    m_world.SetDebugDraw(&m_debugDraw);
    m_debugDraw.SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_centerOfMassBit);
}

void Simulation::init(entt::registry& reg) {
    // iterate over Beams, create beams, create anchors
    reg.view<Beam, Sprite>().each([this, &reg](auto beamEntity, Beam& beam, Sprite& sprite) {
        // body
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(sprite.position.x / WORLD_TO_PHYSICS_RATIO, sprite.position.y / WORLD_TO_PHYSICS_RATIO);
        // mirror horizontally
        bodyDef.angle = glm::radians(sprite.rotation);
        b2Body* beamBody = m_world.CreateBody(&bodyDef);
        reg.emplace<PhysicsBody>(beamEntity, beamBody);

        // fixture
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(sprite.dimensions.x / WORLD_TO_PHYSICS_RATIO / 2,
                            sprite.dimensions.y / WORLD_TO_PHYSICS_RATIO / 2);
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
        revoluteJointDef.localAnchorB.Set(-sprite.dimensions.x / 2 / WORLD_TO_PHYSICS_RATIO, 0);
        m_world.CreateJoint(&revoluteJointDef);

        revoluteJointDef.bodyA = endBody;
        revoluteJointDef.bodyB = beamBody;
        revoluteJointDef.localAnchorA.Set(0, 0);  // axis is at the center of the anchor
        revoluteJointDef.localAnchorB.Set(sprite.dimensions.x / 2 / WORLD_TO_PHYSICS_RATIO, 0);
        m_world.CreateJoint(&revoluteJointDef);
    });
}

void Simulation::reset(entt::registry& reg) {
    reg.view<PhysicsBody>().each([this](auto entity, PhysicsBody& body) {
        m_world.DestroyBody(body.body);
        (void)entity;
    });
}

b2Body* Simulation::getAnchorBody(entt::registry& reg, entt::entity anchorEntity) {
    Anchor& anchor = reg.get<Anchor>(anchorEntity);
    if (reg.has<PhysicsBody>(anchorEntity)) {
        return reg.get<PhysicsBody>(anchorEntity).body;
    } else {
        Sprite& sprite = reg.get<Sprite>(anchorEntity);
        b2BodyDef bodyDef;
        bodyDef.type = anchor.levelAnchor ? b2_staticBody : b2_dynamicBody;
        bodyDef.position.Set(sprite.position.x / WORLD_TO_PHYSICS_RATIO, sprite.position.y / WORLD_TO_PHYSICS_RATIO);
        b2Body* body = m_world.CreateBody(&bodyDef);

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

void Simulation::tick(entt::registry& reg) {
    m_world.Step(TIME_STEP, 7, 7);

    reg.view<Sprite, PhysicsBody>().each([this](auto entity, Sprite& sprite, PhysicsBody& body) {
        std::ignore = entity;
        b2Vec2 position = body.body->GetPosition();
        float angle = body.body->GetAngle();
        sprite.position = glm::vec2(position.x * WORLD_TO_PHYSICS_RATIO, position.y * WORLD_TO_PHYSICS_RATIO);
        sprite.rotation = glm::degrees(angle);
    });
}

void Simulation::debugDraw() {
    m_world.DrawDebugData();
}
