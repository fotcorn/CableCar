#pragma once

#include <SDL_image.h>

#include <string>

#include "Texture.h"

class AssetManager {
  public:
    virtual ~AssetManager() = default;
    virtual std::shared_ptr<SDL_Surface> loadImage(std::string name) = 0;
    virtual std::shared_ptr<Texture> loadTexture(std::string name) = 0;
};
