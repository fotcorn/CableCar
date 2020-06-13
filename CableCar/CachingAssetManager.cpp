#include "CachingAssetManager.h"

std::unordered_map<std::string, std::shared_ptr<SDL_Surface>> CachingAssetManager::surfaceCache;
std::unordered_map<std::string, std::shared_ptr<Texture>> CachingAssetManager::textureCache;

CachingAssetManager::CachingAssetManager(std::unique_ptr<AssetManager> subManager)
    : subManager(std::move(subManager)) {}

std::shared_ptr<SDL_Surface> CachingAssetManager::loadImage(std::string name) {
    auto it = surfaceCache.find(name);
    if (it != surfaceCache.end()) {
        return it->second;
    }

    std::shared_ptr<SDL_Surface> surface = subManager->loadImage(name);

    surfaceCache[name] = surface;
    return surface;
}

std::shared_ptr<Texture> CachingAssetManager::loadTexture(std::string name) {
    auto it = textureCache.find(name);
    if (it != textureCache.end()) {
        return it->second;
    }

    std::shared_ptr<Texture> texture = subManager->loadTexture(name);

    textureCache[name] = texture;
    return texture;
}
