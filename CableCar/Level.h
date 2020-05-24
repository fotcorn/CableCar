#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include <vector>

#include "Texture.h"

class Level {
  public:
    Level(const std::string& path);
    Texture& texture();

  private:
    std::unique_ptr<Texture> m_texture;
};
