#pragma once

#include <SDL2/SDL.h>

#include <entt/entt.hpp>
#include <memory>

#include "Texture.h"

class Input {
  public:
    bool handleInput(entt::registry& reg);

  private:
    // methods
    entt::entity handleMouse(entt::registry& reg);
    void handleKeyEvent(entt::registry& reg, SDL_Event event, entt::entity hoverEntity);

    // data
    entt::entity dragStart = entt::null;
    entt::entity tempDragBeam = entt::null;
    bool mouseButtonDown = false;
};
