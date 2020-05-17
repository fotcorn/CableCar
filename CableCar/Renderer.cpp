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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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

    viewportToVirtualX = static_cast<float>(viewportWidth) / static_cast<float>(VIRTUAL_WIDTH);
    viewportToVirtualY = static_cast<float>(viewportHeight) / static_cast<float>(VIRTUAL_HEIGHT);

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

void Renderer::drawTexture(std::shared_ptr<Texture> texture,
                           int virtualX,
                           int virtualY,
                           int virtualWidth,
                           int virtualHeight) {
    SDL_Rect destinationRect;
    destinationRect.x = virtualX * viewportToVirtualX;
    destinationRect.y = virtualY * viewportToVirtualY;
    destinationRect.w = virtualWidth * viewportToVirtualX;
    destinationRect.h = virtualHeight * viewportToVirtualY;

    SDL_RenderCopy(renderer, texture->texture, nullptr, &destinationRect);
}
