#pragma once

#include <entt/entt.hpp>

class Input {
  public:
    void handleInput();

  private:
    entt::entity currentHover = entt::null;
    entt::entity mouseDownStart = entt::null;
    bool mouseButtonDown = false;
};
