#include "Level.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <cassert>
#include <entt/entt.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Components.h"
#include "Services.h"

constexpr float ANCHOR_SIZE = 20.0f;
constexpr float ANCHOR_RADIUS = ANCHOR_SIZE / 2;

constexpr float BEAM_WIDTH = 10.0f;

enum class Layer : unsigned int {
    BACKGROUND = 100,
    BEAMS = 150,
    ANCHORS = 200,
};

entt::entity Level::createAnchor(const float x, const float y) {
    auto anchor = buildRegistry.create();

    Sprite& sprite = buildRegistry.emplace<Sprite>(anchor);
    sprite.position = glm::vec2(x, y);
    sprite.dimensions = glm::vec2(ANCHOR_SIZE, ANCHOR_SIZE);
    sprite.origin = glm::vec2(ANCHOR_RADIUS, ANCHOR_RADIUS);
    sprite.layer = static_cast<unsigned int>(Layer::BACKGROUND);
    sprite.texture = anchorTexture;

    buildRegistry.emplace<HoverTarget>(anchor, anchorHoverTexture);
    buildRegistry.emplace<CollisionCircle>(anchor, glm::vec2(x, y), ANCHOR_RADIUS);
    buildRegistry.emplace<Anchor>(anchor);
    return anchor;
}

entt::entity Level::createBeam(entt::entity startAnchor, entt::entity endAnchor) {
    auto beam = buildRegistry.create();
    buildRegistry.emplace<Beam>(beam, startAnchor, endAnchor);

    const Sprite& start = buildRegistry.get<Sprite>(startAnchor);
    const Sprite& end = buildRegistry.get<Sprite>(endAnchor);

    const float length = glm::distance(start.position, end.position);
    const glm::vec2 center = (start.position + end.position) / 2.0f;

    const glm::vec2 directionVector = glm::normalize(end.position - start.position);
    const float angleDegree = 90 - glm::degrees(glm::atan(directionVector.x, directionVector.y));

    Sprite& sprite = buildRegistry.emplace<Sprite>(beam);
    sprite.position = center;
    sprite.dimensions = glm::vec2(length, BEAM_WIDTH);
    sprite.origin = glm::vec2(length / 2.0f, BEAM_WIDTH / 2.0f);
    sprite.rotation = angleDegree;
    sprite.layer = static_cast<unsigned int>(Layer::BEAMS);
    sprite.texture = beamTexture;

    return beam;
}

void Level::loadLevel(const std::string& path) {
    buildMode = true;
    buildRegistry.clear();
    simulationRegistry.clear();
    Services::provideRegistry(&buildRegistry);

    anchorTexture = std::make_shared<Texture>("rope_anchor.png");
    anchorHoverTexture = std::make_shared<Texture>("rope_anchor_hover.png");
    beamTexture = std::make_shared<Texture>("pipe.png");

    SDL_Surface* surface = Services::assetManager().loadImage(path);

    // levels should all be transparent pngs with 32bit depth and uniform row access
    assert(surface->format->BytesPerPixel == 4);
    assert(surface->pitch == surface->w * 4);

    SDL_LockSurface(surface);

    uint8_t r, g, b, a;
    Uint32* pixels = (Uint32*)surface->pixels;

    Uint32 whitePixel = SDL_MapRGBA(surface->format, 255, 255, 255, 255);

    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            Uint32* pixel = pixels + y * surface->w + x;  // TODO: use pitch
            SDL_GetRGBA(*pixel, surface->format, &r, &g, &b, &a);
            if (r == 255 && g == 0 && b == 0) {
                createAnchor(x, y);
                *pixel = whitePixel;
            }
            if (r == 0 && g == 255 && b == 0) {
                createAnchor(x, y);
                *pixel = whitePixel;
            }
        }
    }

    SDL_UnlockSurface(surface);

    std::shared_ptr<Texture> levelTexture = std::make_shared<Texture>(surface);

    auto levelEntity = buildRegistry.create();

    Sprite& sprite = buildRegistry.emplace<Sprite>(levelEntity);
    sprite.position = glm::vec2(0, 0);
    sprite.dimensions = glm::vec2(1920, 1080);
    sprite.layer = static_cast<unsigned int>(Layer::ANCHORS);
    sprite.texture = levelTexture;

    SDL_FreeSurface(surface);
}
