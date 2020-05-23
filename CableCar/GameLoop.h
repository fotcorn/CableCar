#pragma once

#include <entt/entt.hpp>
#include <memory>

#include "Level.h"
#include "Renderer.h"
#include "Services.h"

class GameLoop {
  public:
    GameLoop();
    void loop();

  private:
    // methods
    void render();

    // data
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<AssetManager> assetManager;
    std::unique_ptr<Level> level;
    entt::registry registry;
};
