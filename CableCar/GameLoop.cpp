#include "GameLoop.h"

#include <SDL2/SDL.h>

#include <entt/entt.hpp>

#include "Components.h"
#include "FilesystemAssetManager.h"

GameLoop::GameLoop() : buildMode(true) {
    assetManager = std::make_unique<FilesystemAssetManager>("data");
    Services::provideAssetManager(assetManager.get());

    renderer = std::make_unique<Renderer>(1920, 1080);
    Services::provideRenderer(renderer.get());

    Services::provideRegistry(&buildRegistry);

    level = std::make_unique<Level>("level.png");
    Services::provideLevel(level.get());

    input = std::make_unique<Input>();
}

void GameLoop::loop() {
    while (true) {
        if (!input->handleInput()) {
            break;
        }
        renderer->render();
    }
}
