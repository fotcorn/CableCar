#include "GameLoop.h"

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

    level = std::make_unique<Level>("level.png");
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

        int x, y;
        SDL_GetMouseState(&x, &y);
        render(glm::vec2(x, y));
    }
}

void GameLoop::render(glm::vec2 mousePosition) {
    renderer->clear();
    renderer->drawTexture(level->texture(), Transform{glm::vec2(0, 0), glm::vec2(1920, 1080), glm::vec2(0, 0)});

    float minDistance = std::numeric_limits<float>::max();
    entt::entity hoverEntity = entt::null;

    entt::registry& reg = Services::registry();

    auto view = reg.view<Transform, HoverTarget>();
    for (auto entity : view) {
        auto [transform, hoverTarget] = view.get<Transform, HoverTarget>(entity);

        if (reg.has<CollisionCircle>(entity)) {
            auto& collision = reg.get<CollisionCircle>(entity);
            std::unique_ptr<float> distance = collision.collide(mousePosition);
            if (distance && *distance.get() < minDistance) {
                minDistance = *distance.get();
                hoverEntity = entity;
            }
        }
    }

    reg.view<Sprite, Transform>().each([hoverEntity, this](auto entity, auto& sprite, auto& transform) {
        if (hoverEntity == entity) {
            return;
        }
        renderer->drawTexture(*sprite.texture.get(), transform);
    });

    if (reg.valid(hoverEntity)) {
        auto [transform, hoverTarget] = reg.get<Transform, HoverTarget>(hoverEntity);
        if (hoverTarget.hoverTexture) {
            renderer->drawTexture(*hoverTarget.hoverTexture.get(), transform);
        }
    }

    renderer->flip();
}
