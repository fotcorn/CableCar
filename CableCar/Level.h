#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include <vector>

#include "Texture.h"

class Level {
  public:
    Level(const std::string& path);
    Texture& texture();
    /*

    void mouse(ivec2 mousePos, MaterialType materialType) {
        int minPos = INT_MAX;
        ivec2 ropeAnchor;
        if (materialType == MaterialType::Steel) {
            for (ivec2 pos : ropeAnchors) {
                distance = glm::distance(pos, mousePos);
                if (distance < 5 && distance < minPos) {
                    minPos = distance;
                    ropeAnchor = pos;
                }
            }
        }
    }
    */

  private:
    std::vector<glm::ivec2> m_ropeAnchors;
    std::vector<glm::ivec2> m_buildAnchors;
    std::unique_ptr<Texture> m_texture;
};
