#pragma once

#include <Box2D/Box2D.h>

#include <entt/entt.hpp>

class Simulation {
  public:
    Simulation();
    void reset(entt::registry& reg);
    void tick(entt::registry& reg);

  private:
    b2Body* getAnchorBody(entt::registry& reg, entt::entity anchorEntity);

    b2World world;
    b2Body* worldBody;
};
