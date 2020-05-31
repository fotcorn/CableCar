#pragma once

#include <memory>
#include <string>

#include "Texture.h"

class Level {
  public:
    Level(const std::string& path);
    void createAnchor(const float x, const float y);

  private:
    std::shared_ptr<Texture> anchorTexture;
    std::shared_ptr<Texture> anchorHoverTexture;
};
