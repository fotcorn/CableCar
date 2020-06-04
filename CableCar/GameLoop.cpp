#include "GameLoop.h"

#include <SDL2/SDL.h>

#include <entt/entt.hpp>

#include "Components.h"
#include "FilesystemAssetManager.h"

GameLoop::GameLoop() {
    assetManager = std::make_unique<FilesystemAssetManager>("data");
    Services::provideAssetManager(assetManager.get());

    renderer = std::make_unique<Renderer>(1920, 1080);
    Services::provideRenderer(renderer.get());

    level = std::make_unique<Level>();
    Services::provideLevel(level.get());

    input = std::make_unique<Input>();

    level->loadLevel("level.png");
}

void GameLoop::loop() {
    while (true) {
        if (!input->handleInput()) {
            break;
        }
        renderer->render();
    }
}
