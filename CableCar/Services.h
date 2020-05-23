#pragma once

#include <cassert>
#include <entt/entt.hpp>

#include "AssetManager.h"
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
    static void provideAssetManager(AssetManager* assetManager) { _assetManager = assetManager; }
    static void provideRenderer(Renderer* renderer) { _renderer = renderer; }
    static void provideRegistry(entt::registry* registry) { _registry = registry; }

  private:
    static AssetManager* _assetManager;
    static Renderer* _renderer;
    static entt::registry* _registry;
};
