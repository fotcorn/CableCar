#pragma once

#include <SDL2/SDL.h>

#include <entt/entt.hpp>
#include <memory>

#include "Texture.h"

class Input {
  public:
    bool handleInput();

  private:
    // methods
    entt::entity handleMouse();
    void handleKeyEvent(SDL_Event event, entt::entity hoverEntity);

    // data
    entt::entity dragStart = entt::null;
    bool mouseButtonDown = false;
};
