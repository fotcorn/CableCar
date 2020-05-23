#include "GameLoop.h"

#include "FilesystemAssetManager.h"

GameLoop::GameLoop() {
    assetManager = std::make_unique<FilesystemAssetManager>("data");
    Services::provideAssetManager(assetManager.get());

    renderer = std::make_unique<Renderer>(1920, 1080);
    Services::provideRenderer(renderer.get());

    level = std::make_unique<Level>("level.png");

    Services::provideRegistry(&registry);
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

        render();
    }
}

void GameLoop::render() {
    renderer->clear();
    renderer->drawTexture(level->texture(), 0, 0, 1920, 1080);
    renderer->flip();
}
