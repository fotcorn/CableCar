#pragma once

#include <cassert>

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
    static void provideAssetManager(AssetManager* assetManager) { _assetManager = assetManager; }
    static void provideRenderer(Renderer* renderer) { _renderer = renderer; }

  private:
    static AssetManager* _assetManager;
    static Renderer* _renderer;
};
