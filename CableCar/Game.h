#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <string>

#include "AssetManager.h"
#include "Components.h"
#include "Input.h"
#include "Renderer.h"
#include "Simulation.h"
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

    AssetManager& assetManager() { return *m_assetManager; }
    Renderer& renderer() { return *m_renderer; }
    static Game& get() { return *s_game; }

  private:
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<AssetManager> m_assetManager;
    std::unique_ptr<Input> m_input;
    std::unique_ptr<Simulation> m_simulation;

    entt::registry m_buildRegistry;
    entt::registry m_simulationRegistry;
    bool m_buildMode = true;

    entt::entity m_levelEntity;

    static Game* s_game;
};
