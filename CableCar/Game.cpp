#include "Game.h"

#include "CachingAssetManager.h"
#include "FilesystemAssetManager.h"
#include "Level.h"
#include "Services.h"

Game::Game() {
    renderer = std::make_unique<Renderer>(1920, 1080);

    assetManager = std::make_unique<CachingAssetManager>(
        std::make_unique<FilesystemAssetManager>("data", renderer->sdlRenderer()));
    Services::provideAssetManager(assetManager.get());

    Services::provideGame(this);

    input = std::make_unique<Input>();

    loadLevel("level.png");
}

void Game::loop() {
    while (true) {
        if (!input->handleInput(buildRegistry)) {
            break;
        }

        if (buildMode) {
            renderer->render(buildRegistry);
        } else {
            renderer->render(simulationRegistry);
        }
    }
}

void Game::loadLevel(const std::string& path) {
    buildMode = true;
    buildRegistry.clear();
    simulationRegistry.clear();

    levelEntity = Level::load(buildRegistry, path);
}

Game::GameMode Game::gameMode() {
    return buildMode ? GameMode::BUILD_MODE : GameMode::SIMULATION_MODE;
}

void Game::setGameMode(GameMode newMode) {
    if (buildMode && newMode == GameMode::SIMULATION_MODE) {
        simulationRegistry.clear();
        // TODO: copy entities from build registry to simulation registry

        entt::entity simulationLevelEntity = simulationRegistry.create();
        simulationRegistry.emplace<Sprite>(simulationLevelEntity, buildRegistry.get<Sprite>(levelEntity));

        buildRegistry.view<Anchor, Sprite>().each([this](auto entity, Anchor& anchor, Sprite& sprite) {
            std::ignore = entity;
            entt::entity simulationEntity = simulationRegistry.create();
            simulationRegistry.emplace<Anchor>(simulationEntity, anchor);
            simulationRegistry.emplace<Sprite>(simulationEntity, sprite);
        });

        buildMode = false;
    } else if (!buildMode && newMode == GameMode::BUILD_MODE) {
        simulationRegistry.clear();
        buildMode = true;
    }
}
