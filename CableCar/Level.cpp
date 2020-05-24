#include "Level.h"

#include <cassert>
#include <entt/entt.hpp>
#include <iostream>

#include "Components.h"
#include "Services.h"

Level::Level(const std::string& path) {
    SDL_Surface* surface = Services::assetManager().loadImage(path);

    // levels should all be transparent pngs with 32bit depth and uniform row access
    assert(surface->format->BytesPerPixel == 4);
    assert(surface->pitch == surface->w * 4);

    SDL_LockSurface(surface);

    uint8_t r, g, b, a;
    Uint32* pixels = (Uint32*)surface->pixels;

    Uint32 whitePixel = SDL_MapRGBA(surface->format, 255, 255, 255, 255);

    entt::registry& reg = Services::registry();

    auto ropeAnchorTex = std::make_shared<Texture>("rope_anchor.png");
    auto ropeAnchorHoverTex = std::make_shared<Texture>("rope_anchor_hover.png");
    auto buildAnchorTex = std::make_shared<Texture>("build_anchor.png");
    auto buildAnchorHoverTex = std::make_shared<Texture>("build_anchor_hover.png");

    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            Uint32* pixel = pixels + y * surface->w + x;  // TODO: use pitch
            SDL_GetRGBA(*pixel, surface->format, &r, &g, &b, &a);
            if (r == 255 && g == 0 && b == 0) {
                auto ropeAnchor = reg.create();
                reg.emplace<Transform>(ropeAnchor, glm::vec2(x, y), glm::vec2(10.0f, 10.0f), glm::vec2(5.0f, 5.0f));
                reg.emplace<Sprite>(ropeAnchor, ropeAnchorTex);
                reg.emplace<HoverTarget>(ropeAnchor, ropeAnchorHoverTex);
                reg.emplace<CollisionCircle>(ropeAnchor, glm::vec2(5.0f, 5.0f));

                *pixel = whitePixel;
            }
            if (r == 0 && g == 255 && b == 0) {
                auto buildAnchor = reg.create();
                reg.emplace<Transform>(buildAnchor, glm::vec2(x, y), glm::vec2(10.0f, 10.0f), glm::vec2(5.0f, 5.0f));
                reg.emplace<Sprite>(buildAnchor, buildAnchorTex);
                reg.emplace<HoverTarget>(buildAnchor, buildAnchorHoverTex);
                reg.emplace<CollisionCircle>(buildAnchor, glm::vec2(5.0f, 5.0f));
                *pixel = whitePixel;
            }
        }
    }

    SDL_UnlockSurface(surface);

    m_texture = std::make_unique<Texture>(surface);

    SDL_FreeSurface(surface);
}

Texture& Level::texture() {
    return *m_texture;
}
