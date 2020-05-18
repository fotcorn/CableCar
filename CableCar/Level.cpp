#include "Level.h"

#include <cassert>
#include <iostream>

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

    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            Uint32* pixel = pixels + y * surface->w + x;  // TODO: use pitch
            SDL_GetRGBA(*pixel, surface->format, &r, &g, &b, &a);
            if (r == 255 && g == 0 && b == 0) {
                m_ropeAnchors.push_back(glm::ivec2(x, y));
                *pixel = whitePixel;
            }
            if (r == 0 && g == 255 && b == 0) {
                m_buildAnchors.push_back(glm::ivec2(x, y));
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
