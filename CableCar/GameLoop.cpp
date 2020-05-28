#include "GameLoop.h"

#include <SDL2/SDL.h>

#include <entt/entt.hpp>
#include <limits>

#include "Components.h"
#include "FilesystemAssetManager.h"

GameLoop::GameLoop() {
    assetManager = std::make_unique<FilesystemAssetManager>("data");
    Services::provideAssetManager(assetManager.get());

    renderer = std::make_unique<Renderer>(1920, 1080);
    Services::provideRenderer(renderer.get());

    Services::provideRegistry(&registry);

    loadLevel("level.png");
}

void GameLoop::loop() {
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

        input.handleInput();
        renderer->render();
    }
}
