#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <string>

#include "Components.h"
#include "Texture.h"

class Level {
  public:
    void loadLevel(const std::string& path);

    entt::entity createAnchor(const float x, const float y, const bool levelAnchor = false);
    void removeAnchor(entt::entity anchor);

    entt::entity createBeam(const entt::entity startAnchor, const entt::entity endAnchor);
    void updateBeamSprite(const entt::entity beam, const glm::vec2 startPosition, const glm::vec2 endPosition);
    void updateBeamSprite(Sprite& sprite, const glm::vec2 startPosition, const glm::vec2 endPosition);
    void initBeamSprite(const entt::entity beam, const glm::vec2 startPosition, const glm::vec2 endPosition);

    enum GameMode {
        BUILD_MODE,
        SIMULATION_MODE,
    };

    GameMode gameMode();
    void setGameMode(GameMode mode);

  private:
    std::shared_ptr<Texture> anchorTexture;
    std::shared_ptr<Texture> anchorHoverTexture;
    std::shared_ptr<Texture> beamTexture;

    entt::registry buildRegistry;
    entt::registry simulationRegistry;
    bool buildMode = true;

    entt::entity levelEntity;
};
