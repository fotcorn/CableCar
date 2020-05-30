#include "Input.h"

#include <SDL2/SDL.h>

#include "Components.h"
#include "Services.h"

void Input::handleInput() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    glm::vec2 mousePosition(x, y);

    float minDistance = std::numeric_limits<float>::max();
    entt::entity hoverEntity = entt::null;

    entt::registry& reg = Services::registry();

    auto view = reg.view<HoverTarget>();
    for (auto entity : view) {
        if (reg.has<CollisionCircle>(entity)) {
            auto& collision = reg.get<CollisionCircle>(entity);
            std::unique_ptr<float> distance = collision.collide(mousePosition);
            if (distance && *distance.get() < minDistance) {
                minDistance = *distance.get();
                hoverEntity = entity;
            }
        }
    }

    reg.clear<CurrentHover>();
    if (reg.valid(hoverEntity)) {
        reg.emplace<CurrentHover>(hoverEntity);
    }
}
