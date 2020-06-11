#include "Level.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <cassert>
#include <entt/entt.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Services.h"

constexpr float ANCHOR_SIZE = 20.0f;
constexpr float ANCHOR_RADIUS = ANCHOR_SIZE / 2;

constexpr float BEAM_WIDTH = 7.0f;

enum class Layer : unsigned int {
    BACKGROUND = 100,
    BEAMS = 150,
    ANCHORS = 200,
};

entt::entity Level::createAnchor(const float x, const float y, const bool levelAnchor) {
    auto anchor = buildRegistry.create();

    Sprite& sprite = buildRegistry.emplace<Sprite>(anchor);
    sprite.position = glm::vec2(x, y);
    sprite.dimensions = glm::vec2(ANCHOR_SIZE, ANCHOR_SIZE);
    sprite.origin = glm::vec2(ANCHOR_RADIUS, ANCHOR_RADIUS);
    sprite.layer = static_cast<unsigned int>(Layer::BACKGROUND);
    sprite.texture = anchorTexture;

    buildRegistry.emplace<HoverTarget>(anchor, anchorHoverTexture);
    buildRegistry.emplace<CollisionCircle>(anchor, glm::vec2(x, y), ANCHOR_RADIUS);
    buildRegistry.emplace<Anchor>(anchor, levelAnchor);
    return anchor;
}

void Level::removeAnchor(entt::entity anchor) {
    Anchor& anchorComponent = buildRegistry.get<Anchor>(anchor);
    if (anchorComponent.levelAnchor) {
        // anchors loaded from the level cannot be deleted
        return;
    }

    buildRegistry.view<Beam>().each([this, anchor](auto entity, Beam& beam) {
        if (beam.start == anchor || beam.end == anchor) {
            buildRegistry.destroy(entity);
        }
    });

    buildRegistry.destroy(anchor);
}

void Level::updateBeamSprite(const entt::entity beam, const glm::vec2 startPosition, const glm::vec2 endPosition) {
    Sprite& sprite = buildRegistry.get<Sprite>(beam);
    updateBeamSprite(sprite, startPosition, endPosition);
}

void Level::updateBeamSprite(Sprite& sprite, const glm::vec2 startPosition, const glm::vec2 endPosition) {
    const float length = glm::distance(startPosition, endPosition);
    const glm::vec2 center = (startPosition + endPosition) / 2.0f;

    const glm::vec2 directionVector = glm::normalize(endPosition - startPosition);
    const float angleDegree = 90 - glm::degrees(glm::atan(directionVector.x, directionVector.y));

    sprite.position = center;
    sprite.dimensions = glm::vec2(length, BEAM_WIDTH);
    sprite.origin = glm::vec2(length / 2.0f, BEAM_WIDTH / 2.0f);
    sprite.rotation = angleDegree;
}

void Level::initBeamSprite(const entt::entity beam, const glm::vec2 startPosition, const glm::vec2 endPosition) {
    Sprite& sprite = buildRegistry.emplace<Sprite>(beam);
    sprite.layer = static_cast<unsigned int>(Layer::BEAMS);
    sprite.texture = beamTexture;
    updateBeamSprite(sprite, startPosition, endPosition);
}

entt::entity Level::createBeam(const entt::entity startAnchor, const entt::entity endAnchor) {
    auto beam = buildRegistry.create();
    buildRegistry.emplace<Beam>(beam, startAnchor, endAnchor);

    const Sprite& start = buildRegistry.get<Sprite>(startAnchor);
    const Sprite& end = buildRegistry.get<Sprite>(endAnchor);

    initBeamSprite(beam, start.position, end.position);

    return beam;
}

void Level::loadLevel(const std::string& path) {
    buildMode = true;
    buildRegistry.clear();
    simulationRegistry.clear();
    Services::provideRegistry(&buildRegistry);

    anchorTexture = Texture::load("rope_anchor.png");
    anchorHoverTexture = Texture::load("rope_anchor_hover.png");
    beamTexture = Texture::load("pipe.png");

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
                createAnchor(x, y, true);
                *pixel = whitePixel;
            }
            if (r == 0 && g == 255 && b == 0) {
                createAnchor(x, y, true);
                *pixel = whitePixel;
            }
        }
    }

    SDL_UnlockSurface(surface);

    levelEntity = buildRegistry.create();

    Sprite& sprite = buildRegistry.emplace<Sprite>(levelEntity);
    sprite.position = glm::vec2(0, 0);
    sprite.dimensions = glm::vec2(1920, 1080);
    sprite.layer = static_cast<unsigned int>(Layer::ANCHORS);
    sprite.texture = Texture::load(path);

    SDL_FreeSurface(surface);
}

Level::GameMode Level::gameMode() {
    return buildMode ? GameMode::BUILD_MODE : GameMode::SIMULATION_MODE;
}

void Level::setGameMode(GameMode newMode) {
    if (buildMode && newMode == GameMode::SIMULATION_MODE) {
        simulationRegistry.clear();
        // TODO: copy entities from build registry to simulation registry

        entt::entity simulationLevelEntity = simulationRegistry.create();
        simulationRegistry.emplace<Sprite>(simulationLevelEntity, buildRegistry.get<Sprite>(levelEntity));

        buildRegistry.view<Anchor, Sprite>().each([this](auto entity, Anchor& anchor, Sprite& sprite) {
            std::ignore = entity;
            entt::entity simulationEntity = simulationRegistry.create();
            simulationRegistry.emplace<Anchor>(simulationEntity, anchor);
            simulationRegistry.emplace<Sprite>(simulationEntity, sprite);
        });


        Services::provideRegistry(&simulationRegistry);
        buildMode = false;
    } else if (!buildMode && newMode == GameMode::BUILD_MODE) {
        simulationRegistry.clear();
        Services::provideRegistry(&buildRegistry);
        buildMode = true;
    }
}
