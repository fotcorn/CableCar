#pragma once

#include <SDL2/SDL.h>

#include "Components.h"
#include "Texture.h"

constexpr int VIRTUAL_WIDTH = 1920;
constexpr int VIRTUAL_HEIGHT = 1080;
constexpr float VIRTUAL_ASPECT_RATIO = static_cast<float>(VIRTUAL_WIDTH) / static_cast<float>(VIRTUAL_HEIGHT);

class Renderer {
  public:
    Renderer(int screenWidth, int screenHeight);
    ~Renderer();

    SDL_Renderer* sdlRenderer() { return renderer; }

    void clear();
    void flip();

    void drawTexture(const Texture& texture, const Transform& transform);

  private:
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
    int viewportWidth;
    int viewportHeight;
    float viewportToVirtualX;
    float viewportToVirtualY;
};
