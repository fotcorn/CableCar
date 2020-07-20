#include "Game.h"

#include <unordered_map>

#include "CachingAssetManager.h"
#include "FilesystemAssetManager.h"
#include "Level.h"

Game* Game::s_game = nullptr;

Game::Game() {
    s_game = this;

    m_renderer = std::make_unique<Renderer>(1920, 1080);

    m_assetManager = std::make_unique<CachingAssetManager>(
        std::make_unique<FilesystemAssetManager>("data", m_renderer->sdlRenderer()));

    m_input = std::make_unique<Input>();
    m_simulation = std::make_unique<Simulation>();

    loadLevel("level.png");
}

void Game::loop() {
    while (true) {
        if (!m_input->handleInput(m_buildRegistry)) {
            break;
        }

        if (m_buildMode) {
            m_renderer->clear();
            m_renderer->renderWorld(m_buildRegistry);
            m_renderer->flip();
        } else {
            m_simulation->tick(m_simulationRegistry);
            m_renderer->clear();
            m_renderer->renderWorld(m_simulationRegistry);
            m_simulation->debugDraw();
            m_renderer->flip();
        }
    }
}

void Game::loadLevel(const std::string& path) {
    m_buildMode = true;
    m_buildRegistry.clear();
    m_simulationRegistry.clear();

    m_levelEntity = Level::load(m_buildRegistry, path);
}

Game::GameMode Game::gameMode() {
    return m_buildMode ? GameMode::BUILD_MODE : GameMode::SIMULATION_MODE;
}

void Game::setGameMode(GameMode newMode) {
    if (m_buildMode && newMode == GameMode::SIMULATION_MODE) {
        m_simulationRegistry.clear();

        entt::entity simulationLevelEntity = m_simulationRegistry.create();
        m_simulationRegistry.emplace<Sprite>(simulationLevelEntity, m_buildRegistry.get<Sprite>(m_levelEntity));

        std::unordered_map<entt::entity, entt::entity> buildToSimulationMap;

        m_buildRegistry.view<Anchor, Sprite>().each(
            [this, &buildToSimulationMap](auto entity, Anchor& anchor, Sprite& sprite) {
                entt::entity simulationEntity = m_simulationRegistry.create();
                m_simulationRegistry.emplace<Anchor>(simulationEntity, anchor);
                m_simulationRegistry.emplace<Sprite>(simulationEntity, sprite);

                buildToSimulationMap[entity] = simulationEntity;
            });

        m_buildRegistry.view<Beam, Sprite>().each([this, &buildToSimulationMap](auto entity, Beam& beam, Sprite& sprite) {
            std::ignore = entity;
            entt::entity simulationEntity = m_simulationRegistry.create();
            m_simulationRegistry.emplace<Beam>(simulationEntity, buildToSimulationMap[beam.start],
                                             buildToSimulationMap[beam.end]);
            m_simulationRegistry.emplace<Sprite>(simulationEntity, sprite);
        });

        m_simulation->reset(m_simulationRegistry);

        m_buildMode = false;
    } else if (!m_buildMode && newMode == GameMode::BUILD_MODE) {
        m_simulationRegistry.clear();
        m_buildMode = true;
    }
}
