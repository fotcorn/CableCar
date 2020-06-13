#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <string>

#include "AssetManager.h"
#include "Components.h"
#include "Input.h"
#include "Renderer.h"
#include "Texture.h"

class Game {
  public:
    Game();
    void loadLevel(const std::string& path);

    enum GameMode {
        BUILD_MODE,
        SIMULATION_MODE,
    };

    GameMode gameMode();
    void setGameMode(GameMode mode);

    void loop();

  private:
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<AssetManager> assetManager;
    std::unique_ptr<Input> input;

    entt::registry buildRegistry;
    entt::registry simulationRegistry;
    bool buildMode = true;

    entt::entity levelEntity;
};
