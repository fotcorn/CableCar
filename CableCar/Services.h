#pragma once

#include <cassert>
#include <entt/entt.hpp>

#include "AssetManager.h"
#include "Level.h"
#include "Renderer.h"

class Services {
  public:
    static AssetManager& assetManager() {
        assert(_assetManager != nullptr);
        return *_assetManager;
    }
    static Renderer& renderer() {
        assert(_renderer != nullptr);
        return *_renderer;
    }
    static entt::registry& registry() {
        assert(_registry != nullptr);
        return *_registry;
    }
    static Level& level() {
        assert(_level != nullptr);
        return *_level;
    }
    static void provideAssetManager(AssetManager* assetManager) { _assetManager = assetManager; }
    static void provideRenderer(Renderer* renderer) { _renderer = renderer; }
    static void provideRegistry(entt::registry* registry) { _registry = registry; }
    static void provideLevel(Level* level) { _level = level; }

  private:
    static AssetManager* _assetManager;
    static Renderer* _renderer;
    static entt::registry* _registry;
    static Level* _level;
};
