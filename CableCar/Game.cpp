#include "Game.h"

#include <unordered_map>

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
    simulation = std::make_unique<Simulation>();

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
            simulation->tick(simulationRegistry);
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

        entt::entity simulationLevelEntity = simulationRegistry.create();
        simulationRegistry.emplace<Sprite>(simulationLevelEntity, buildRegistry.get<Sprite>(levelEntity));

        std::unordered_map<entt::entity, entt::entity> buildToSimulationMap;

        buildRegistry.view<Anchor, Sprite>().each(
            [this, &buildToSimulationMap](auto entity, Anchor& anchor, Sprite& sprite) {
                entt::entity simulationEntity = simulationRegistry.create();
                simulationRegistry.emplace<Anchor>(simulationEntity, anchor);
                simulationRegistry.emplace<Sprite>(simulationEntity, sprite);

                buildToSimulationMap[entity] = simulationEntity;
            });

        buildRegistry.view<Beam, Sprite>().each([this, &buildToSimulationMap](auto entity, Beam& beam, Sprite& sprite) {
            std::ignore = entity;
            entt::entity simulationEntity = simulationRegistry.create();
            simulationRegistry.emplace<Beam>(simulationEntity, buildToSimulationMap[beam.start],
                                             buildToSimulationMap[beam.end]);
            simulationRegistry.emplace<Sprite>(simulationEntity, sprite);
        });

        simulation->reset(simulationRegistry);

        buildMode = false;
    } else if (!buildMode && newMode == GameMode::BUILD_MODE) {
        simulationRegistry.clear();
        buildMode = true;
    }
}
