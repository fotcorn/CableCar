#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <string>

#include "Texture.h"

class Level {
  public:
    void loadLevel(const std::string& path);
    entt::entity createAnchor(const float x, const float y);
    entt::entity createBeam(entt::entity startAnchor, entt::entity endAnchor);

  private:
    std::shared_ptr<Texture> anchorTexture;
    std::shared_ptr<Texture> anchorHoverTexture;
    std::shared_ptr<Texture> beamTexture;

    entt::registry buildRegistry;
    entt::registry simulationRegistry;
    bool buildMode = true;
};
