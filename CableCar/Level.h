#include <entt/entt.hpp>
#include <glm/vec2.hpp>

#include "Components.h"

namespace Level {
entt::entity load(entt::registry& reg, const std::string& path);
entt::entity createAnchor(entt::registry& reg, const float x, const float y, const bool levelAnchor = false);
void removeAnchor(entt::registry& reg, entt::entity anchor);

entt::entity createBeam(entt::registry& reg, const entt::entity startAnchor, const entt::entity endAnchor);
void initBeamSprite(entt::registry& reg,
                    const entt::entity beam,
                    const glm::vec2 startPosition,
                    const glm::vec2 endPosition);
void updateBeamSprite(entt::registry& reg,
                      const entt::entity beam,
                      const glm::vec2 startPosition,
                      const glm::vec2 endPosition);
void updateBeamSprite(Sprite& sprite, const glm::vec2 startPosition, const glm::vec2 endPosition);
}  // namespace Level
