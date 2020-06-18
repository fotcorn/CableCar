#include "Simulation.h"

constexpr float TIME_STEP = 1000.0f / 60.0f;

Simulation::Simulation() : world(b2Vec2(0.0f, -10.0f)) {}

void Simulation::reset(entt::registry& reg) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    b2Body* body = world.CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
}

void Simulation::tick(entt::registry& reg) {
    world.Step(TIME_STEP, 7, 7);

    // b2Vec2 position = body->GetPosition();
    // float angle = body->GetAngle();
    // printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
}
