#pragma once

#include "AssetManager.h"

class CachingAssetManager : public AssetManager {
  public:
    CachingAssetManager(std::unique_ptr<AssetManager> subManager);
    std::shared_ptr<SDL_Surface> loadImage(std::string name) override;
    std::shared_ptr<Texture> loadTexture(std::string name) override;

  private:
    std::unique_ptr<AssetManager> subManager;
    static std::unordered_map<std::string, std::shared_ptr<Texture>> textureCache;
    static std::unordered_map<std::string, std::shared_ptr<SDL_Surface>> surfaceCache;
};
