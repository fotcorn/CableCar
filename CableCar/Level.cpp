#include "Level.h"

#include "Services.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <cassert>
#include <glm/gtx/vector_angle.hpp>

namespace Level {

constexpr float ANCHOR_SIZE = 20.0f;
constexpr float ANCHOR_RADIUS = ANCHOR_SIZE / 2;

constexpr float BEAM_WIDTH = 7.0f;

enum class Layer : unsigned int {
    BACKGROUND = 100,
    BEAMS = 150,
    ANCHORS = 200,
};

const char* ANCHOR_TEXTURE = "rope_anchor.png";
const char* ANCHOR_HOVER_TEXTURE = "rope_anchor_hover.png";
const char* BEAM_TEXTURE = "pipe.png";

entt::entity load(entt::registry& reg, const std::string& path) {
    std::shared_ptr<SDL_Surface> surface = Services::assetManager().loadImage(path);

    // levels should all be transparent pngs with 32bit depth and uniform row access
    assert(surface->format->BytesPerPixel == 4);
    assert(surface->pitch == surface->w * 4);

    SDL_LockSurface(surface.get());

    uint8_t r, g, b, a;
    Uint32* pixels = (Uint32*)surface->pixels;

    Uint32 whitePixel = SDL_MapRGBA(surface->format, 255, 255, 255, 255);

    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            Uint32* pixel = pixels + y * surface->w + x;  // TODO: use pitch
            SDL_GetRGBA(*pixel, surface->format, &r, &g, &b, &a);
            if (r == 255 && g == 0 && b == 0) {
                createAnchor(reg, x, y, true);
                *pixel = whitePixel;
            }
            if (r == 0 && g == 255 && b == 0) {
                createAnchor(reg, x, y, true);
                *pixel = whitePixel;
            }
        }
    }

    SDL_UnlockSurface(surface.get());

    entt::entity levelEntity = reg.create();

    Sprite& sprite = reg.emplace<Sprite>(levelEntity);
    sprite.position = glm::vec2(0, 0);
    sprite.dimensions = glm::vec2(1920, 1080);
    sprite.layer = static_cast<unsigned int>(Layer::ANCHORS);
    sprite.texture = Services::assetManager().loadTexture(path);

    return levelEntity;
}

entt::entity createAnchor(entt::registry& reg, const float x, const float y, const bool levelAnchor) {
    auto anchor = reg.create();

    Sprite& sprite = reg.emplace<Sprite>(anchor);
    sprite.position = glm::vec2(x, y);
    sprite.dimensions = glm::vec2(ANCHOR_SIZE, ANCHOR_SIZE);
    sprite.origin = glm::vec2(ANCHOR_RADIUS, ANCHOR_RADIUS);
    sprite.layer = static_cast<unsigned int>(Layer::BACKGROUND);
    sprite.texture = Services::assetManager().loadTexture(ANCHOR_TEXTURE);

    reg.emplace<HoverTarget>(anchor, Services::assetManager().loadTexture(ANCHOR_HOVER_TEXTURE));
    reg.emplace<CollisionCircle>(anchor, glm::vec2(x, y), ANCHOR_RADIUS);
    reg.emplace<Anchor>(anchor, levelAnchor);
    return anchor;
}

void removeAnchor(entt::registry& reg, entt::entity anchor) {
    Anchor& anchorComponent = reg.get<Anchor>(anchor);
    if (anchorComponent.levelAnchor) {
        // anchors loaded from the level cannot be deleted
        return;
    }

    reg.view<Beam>().each([anchor, &reg](auto entity, Beam& beam) {
        if (beam.start == anchor || beam.end == anchor) {
            reg.destroy(entity);
        }
    });

    reg.destroy(anchor);
}

void updateBeamSprite(entt::registry& reg,
                      const entt::entity beam,
                      const glm::vec2 startPosition,
                      const glm::vec2 endPosition) {
    Sprite& sprite = reg.get<Sprite>(beam);
    updateBeamSprite(sprite, startPosition, endPosition);
}

void updateBeamSprite(Sprite& sprite, const glm::vec2 startPosition, const glm::vec2 endPosition) {
    const float length = glm::distance(startPosition, endPosition);
    const glm::vec2 center = (startPosition + endPosition) / 2.0f;

    const glm::vec2 directionVector = glm::normalize(endPosition - startPosition);
    const float angleDegree = 90 - glm::degrees(glm::atan(directionVector.x, directionVector.y));

    sprite.position = center;
    sprite.dimensions = glm::vec2(length, BEAM_WIDTH);
    sprite.origin = glm::vec2(length / 2.0f, BEAM_WIDTH / 2.0f);
    sprite.rotation = angleDegree;
}

void initBeamSprite(entt::registry& reg,
                    const entt::entity beam,
                    const glm::vec2 startPosition,
                    const glm::vec2 endPosition) {
    Sprite& sprite = reg.emplace<Sprite>(beam);
    sprite.layer = static_cast<unsigned int>(Layer::BEAMS);
    sprite.texture = Services::assetManager().loadTexture(BEAM_TEXTURE);
    updateBeamSprite(sprite, startPosition, endPosition);
}

entt::entity createBeam(entt::registry& reg, const entt::entity startAnchor, const entt::entity endAnchor) {
    auto beam = reg.create();
    reg.emplace<Beam>(beam, startAnchor, endAnchor);

    const Sprite& start = reg.get<Sprite>(startAnchor);
    const Sprite& end = reg.get<Sprite>(endAnchor);

    initBeamSprite(reg, beam, start.position, end.position);

    return beam;
}
}  // namespace Level
