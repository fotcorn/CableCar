#pragma once

#include <entt/entt.hpp>
#include <memory>

#include "Texture.h"

class Input {
  public:
    Input();
    void handleInput();

  private:
    entt::entity dragStart = entt::null;
    bool mouseButtonDown = false;

    std::shared_ptr<Texture> pipeTexture;
};
