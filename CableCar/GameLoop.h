#pragma once

#include <entt/entt.hpp>
#include <memory>

#include "Game.h"
#include "Input.h"
#include "Renderer.h"
#include "Services.h"

class GameLoop {
  public:
    GameLoop();
    void loop();

  private:
    // methods
    void render(glm::vec2 mousePosition);
    void handleKeyEvent(SDL_Event event);

    // data
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<AssetManager> assetManager;
    std::unique_ptr<Game> game;
    std::unique_ptr<Input> input;
};
