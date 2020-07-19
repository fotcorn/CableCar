#pragma once

#include <SDL2/SDL.h>

#include <entt/entt.hpp>

#include "Components.h"
#include "Texture.h"
#include "GlobalConstants.h"

constexpr float VIRTUAL_ASPECT_RATIO = static_cast<float>(WORLD_WIDTH) / static_cast<float>(WORLD_HEIGHT);

class Renderer {
  public:
    Renderer(int screenWidth, int screenHeight);
    ~Renderer();

    SDL_Renderer* sdlRenderer() { return renderer; }

    void render(entt::registry& reg);

  private:
    void clear();
    void flip();
    void drawTexture(const Sprite& sprite);
    void drawTexture(const Sprite& sprite, const Texture& overrideTexture);

    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
    int viewportWidth;
    int viewportHeight;
    float viewportToVirtualX;
    float viewportToVirtualY;
};
