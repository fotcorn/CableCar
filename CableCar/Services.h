#pragma once

#include <cassert>
#include <entt/entt.hpp>

#include "AssetManager.h"
#include "Game.h"
#include "Renderer.h"

class Services {
  public:
    static AssetManager& assetManager() {
        assert(_assetManager != nullptr);
        return *_assetManager;
    }
    static entt::registry& registry() {
        assert(_registry != nullptr);
        return *_registry;
    }
    static Game& game() {
        assert(_game != nullptr);
        return *_game;
    }
    static void provideAssetManager(AssetManager* assetManager) { _assetManager = assetManager; }
    static void provideRegistry(entt::registry* registry) { _registry = registry; }
    static void provideGame(Game* game) { _game = game; }

  private:
    static AssetManager* _assetManager;
    static entt::registry* _registry;
    static Game* _game;
};
