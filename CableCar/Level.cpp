#include "Level.h"

#include <cassert>
#include <entt/entt.hpp>

#include "Components.h"
#include "Services.h"

constexpr float ANCHOR_SIZE = 20.0f;
constexpr float ANCHOR_RADIUS = ANCHOR_SIZE / 2;

enum class Layer : unsigned int {
    BACKGROUND = 100,
    ANCHORS = 200,

};

void Level::createAnchor(const float x, const float y) {
    entt::registry& reg = Services::registry();
    auto anchor = reg.create();
    reg.emplace<Sprite>(anchor, glm::vec2(x, y), glm::vec2(ANCHOR_SIZE, ANCHOR_SIZE),
                        glm::vec2(ANCHOR_RADIUS, ANCHOR_RADIUS), static_cast<unsigned int>(Layer::BACKGROUND),
                        anchorTexture);
    reg.emplace<HoverTarget>(anchor, anchorHoverTexture);
    reg.emplace<CollisionCircle>(anchor, glm::vec2(x, y), ANCHOR_RADIUS);
}

Level::Level(const std::string& path) {
    entt::registry& reg = Services::registry();

    anchorTexture = std::make_shared<Texture>("rope_anchor.png");
    anchorHoverTexture = std::make_shared<Texture>("rope_anchor_hover.png");

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

    auto levelEntity = reg.create();
    reg.emplace<Sprite>(levelEntity, glm::vec2(0, 0), glm::vec2(1920, 1080), glm::vec2(0),
                        static_cast<unsigned int>(Layer::ANCHORS), levelTexture);

    SDL_FreeSurface(surface);
}
