#include "Renderer.h"

#include <cmath>
#include <stdexcept>
#include <string>

#include "SDL2_gfxPrimitives.h"

Renderer::Renderer(int screenWidth, int screenHeight) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(std::string("Failed to initialize video: ") + SDL_GetError());
    }

    window = SDL_CreateWindow("CableCar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        throw std::runtime_error(std::string("Failed to create window: ") + SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        throw std::runtime_error(std::string("Failed to create renderer: ") + SDL_GetError());
    }

    // based on http://www.david-amador.com/2013/04/opengl-2d-independent-resolution-rendering/
    viewportWidth = screenWidth;
    viewportHeight = static_cast<int>(viewportWidth / VIRTUAL_ASPECT_RATIO + 0.5f);

    if (viewportHeight > screenHeight) {
        viewportHeight = screenHeight;
        viewportWidth = static_cast<int>(viewportHeight * VIRTUAL_ASPECT_RATIO + 0.5f);
    }

    viewportToVirtualX = static_cast<float>(viewportWidth) / static_cast<float>(WORLD_WIDTH);
    viewportToVirtualY = static_cast<float>(viewportHeight) / static_cast<float>(WORLD_HEIGHT);

    int viewportX = (screenWidth / 2) - (viewportWidth / 2);
    int viewportY = (screenHeight / 2) - (viewportHeight / 2);

    SDL_Rect viewport;
    viewport.x = viewportX;
    viewport.y = viewportY;
    viewport.w = viewportWidth;
    viewport.h = viewportHeight;
    SDL_RenderSetViewport(renderer, &viewport);
}

Renderer::~Renderer() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::clear() {
    SDL_RenderClear(renderer);
}

void Renderer::flip() {
    SDL_RenderPresent(renderer);
}

void Renderer::drawTexture(const Sprite& sprite, const Texture& overrideTexture) {
    float xPosition = sprite.position.x - sprite.origin.x;
    float yPosition = sprite.position.y - sprite.origin.y;

    SDL_Rect destinationRect;
    destinationRect.x = xPosition * viewportToVirtualX;
    destinationRect.y = yPosition * viewportToVirtualY;
    destinationRect.w = sprite.dimensions.x * viewportToVirtualX;
    destinationRect.h = sprite.dimensions.y * viewportToVirtualY;

    SDL_Point center{static_cast<int>(std::lroundf(sprite.origin.x)), static_cast<int>(std::lroundf(sprite.origin.y))};

    SDL_RenderCopyEx(renderer, overrideTexture.m_texture, nullptr, &destinationRect, sprite.rotation, &center,
                     SDL_FLIP_NONE);
}

void Renderer::drawTexture(const Sprite& sprite) {
    drawTexture(sprite, *sprite.texture.get());
}

void Renderer::renderWorld(entt::registry& reg) {
    entt::entity hoverEntity = entt::null;
    auto view = reg.view<CurrentHover>();
    if (const auto it = view.begin(); it != view.end()) {
        hoverEntity = *it;
    }

    // sort sprites by layer for correct rendering (painter algorithm)
    // we insert only a small number of new entities per frame, so insertation sort should be faster
    // than other algorithms.
    reg.sort<Sprite>([](const auto& lhs, const auto& rhs) { return lhs.layer > rhs.layer; }, entt::insertion_sort{});

    reg.view<Sprite>().each([hoverEntity, this](auto entity, auto& sprite) {
        if (hoverEntity == entity) {
            return;  // render hoverEntity last
        }
        drawTexture(sprite);
    });

    if (reg.valid(hoverEntity)) {
        auto [sprite, hoverTarget] = reg.get<Sprite, HoverTarget>(hoverEntity);
        drawTexture(sprite, *hoverTarget.hoverTexture.get());
    }
}

int Renderer::polygonRGBA(Sint16* vx, Sint16* vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    for (int i = 0; i < n; i++) {
        vx[i] = vx[i] * viewportToVirtualX;
        vy[i] = vy[i] * viewportToVirtualY;
    }
    return ::polygonRGBA(renderer, vx, vy, n, r, g, b, a);
}
int Renderer::filledPolygonRGBA(Sint16* vx, Sint16* vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    for (int i = 0; i < n; i++) {
        vx[i] = vx[i] * viewportToVirtualX;
        vy[i] = vy[i] * viewportToVirtualY;
    }
    return ::filledPolygonRGBA(renderer, vx, vy, n, r, g, b, a);
}
int Renderer::circleRGBA(Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return ::circleRGBA(renderer, x * viewportToVirtualX, y * viewportToVirtualY, rad, r, g, b, a);
}
int Renderer::filledCircleRGBA(Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return ::filledCircleRGBA(renderer, x * viewportToVirtualX, y * viewportToVirtualY, rad, r, g, b, a);
}
int Renderer::lineRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return ::lineRGBA(renderer, x1 * viewportToVirtualX, y1 * viewportToVirtualY, x2 * viewportToVirtualX,
                      y2 * viewportToVirtualY, r, g, b, a);
}
int Renderer::pixelRGBA(Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return ::pixelRGBA(renderer, x * viewportToVirtualX, y * viewportToVirtualY, r, g, b, a);
}
