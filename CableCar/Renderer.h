#pragma once

#include <SDL2/SDL.h>

#include <entt/entt.hpp>

#include "Components.h"
#include "GlobalConstants.h"
#include "Texture.h"

constexpr float VIRTUAL_ASPECT_RATIO = static_cast<float>(WORLD_WIDTH) / static_cast<float>(WORLD_HEIGHT);

class Renderer {
  public:
    Renderer(int screenWidth, int screenHeight);
    ~Renderer();

    SDL_Renderer* sdlRenderer() { return renderer; }

    void clear();
    void flip();

    void renderWorld(entt::registry& reg);

    int polygonRGBA(Sint16* vx, Sint16* vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    int filledPolygonRGBA(Sint16* vx, Sint16* vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    int circleRGBA(Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    int filledCircleRGBA(Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    int lineRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    int pixelRGBA(Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

  private:
    void drawTexture(const Sprite& sprite);
    void drawTexture(const Sprite& sprite, const Texture& overrideTexture);

    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
    int viewportWidth;
    int viewportHeight;
    float viewportToVirtualX;
    float viewportToVirtualY;
};
