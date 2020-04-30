#pragma once

#include "Texture.h"

#include <SDL2/SDL.h>

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

    void drawTexture(std::shared_ptr<Texture> texture, int virtualX, int virtualY, int width, int height);

  private:
    SDL_Renderer* renderer;
    SDL_Window* window;
};
