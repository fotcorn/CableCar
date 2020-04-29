#include <SDL2/SDL.h>

#include <iostream>

#include "Texture.h"

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize video: " << SDL_GetError()
                  << std::endl;
        return 1;
    }

    SDL_Window* window =
        SDL_CreateWindow("CableCar", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, 1024, 786, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError();
        return 1;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, 4, SDL_RENDERER_ACCELERATED);

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
