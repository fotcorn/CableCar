#include <SDL2/SDL.h>

#include <iostream>

#include "Texture.h"

constexpr int VIRTUAL_WIDTH = 1920;
constexpr int VIRTUAL_HEIGHT = 1080;
constexpr float VIRTUAL_ASPECT_RATIO = static_cast<float>(VIRTUAL_WIDTH) / static_cast<float>(VIRTUAL_HEIGHT);

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize video: " << SDL_GetError() << std::endl;
        return 1;
    }

    // weird resolution to test aspect ratio and resolution independent rendering
    int screenWidth = 1700;
    int screenHeight = 900;

    SDL_Window* window = SDL_CreateWindow("CableCar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth,
                                          screenHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, 4, SDL_RENDERER_ACCELERATED);

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

    Texture::init(renderer);

    std::shared_ptr<Texture> image = Texture::loadImage("level.png");

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }

        SDL_RenderClear(renderer);

        image->render();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
