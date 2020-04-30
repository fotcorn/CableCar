#include "Renderer.h"

#include <stdexcept>
#include <string>

Renderer::Renderer(int screenWidth, int screenHeight) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(std::string("Failed to initialize video: ") + SDL_GetError());
    }

    window = SDL_CreateWindow("CableCar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        throw std::runtime_error(std::string("Failed to create window: ") + SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, 4, SDL_RENDERER_ACCELERATED);

    // based on http://www.david-amador.com/2013/04/opengl-2d-independent-resolution-rendering/
    int width = screenWidth;
    int height = static_cast<int>(width / VIRTUAL_ASPECT_RATIO + 0.5f);

    if (height > screenHeight) {
        height = screenHeight;
        width = static_cast<int>(height * VIRTUAL_ASPECT_RATIO + 0.5f);
    }

    // set up the new viewport centered in the backbuffer
    int viewportX = (screenWidth / 2) - (width / 2);
    int viewportY = (screenHeight / 2) - (height / 2);

    SDL_Rect viewport;
    viewport.x = viewportX;
    viewport.y = viewportY;
    viewport.w = width;
    viewport.h = height;
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

void Renderer::drawTexture(std::shared_ptr<Texture> texture, int virtualX, int virtualY, int width, int height) {
    SDL_RenderCopy(renderer, texture->texture, nullptr, nullptr);
}
