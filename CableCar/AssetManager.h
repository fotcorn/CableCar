#pragma once

#include <SDL_image.h>

#include <string>

class AssetManager {
  public:
    virtual ~AssetManager() = default;
    virtual SDL_Surface* loadImage(std::string name) = 0;
};
