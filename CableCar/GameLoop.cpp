#include "GameLoop.h"

#include <SDL2/SDL.h>

#include <entt/entt.hpp>

#include "CachingAssetManager.h"
#include "Components.h"
#include "FilesystemAssetManager.h"

GameLoop::GameLoop() {
    renderer = std::make_unique<Renderer>(1920, 1080);

    assetManager = std::make_unique<CachingAssetManager>(
        std::make_unique<FilesystemAssetManager>("data", renderer->sdlRenderer()));
    Services::provideAssetManager(assetManager.get());

    game = std::make_unique<Game>();
    Services::provideGame(game.get());

    input = std::make_unique<Input>();

    game->loadLevel("level.png");
}

void GameLoop::loop() {
    while (true) {
        if (!input->handleInput()) {
            break;
        }
        renderer->render();
    }
}
