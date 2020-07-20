#pragma once

#include <Box2D/Box2D.h>

#include <entt/entt.hpp>

#include "Box2DDebugDraw.h"

class Simulation {
  public:
    Simulation();
    void reset(entt::registry& reg);
    void tick(entt::registry& reg);
    void debugDraw();

  private:
    b2Body* getAnchorBody(entt::registry& reg, entt::entity anchorEntity);

    b2World m_world;
    Box2DDebugDraw m_debugDraw;
};
