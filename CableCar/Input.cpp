#include "Input.h"

#include <SDL2/SDL.h>

#include "Components.h"
#include "Services.h"

Input::Input() {
    std::shared_ptr<Texture> pipeTexture = std::make_shared<Texture>("pipe.png");
}

void Input::handleInput() {
    int x, y;
    Uint32 mouseState = SDL_GetMouseState(&x, &y);
    glm::vec2 mousePosition(x, y);

    // find current hovered anchor (if any)
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

    // mouse was not pressed before
    if (!mouseButtonDown && mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (reg.valid(hoverEntity)) {
            dragStart = hoverEntity;
        } else {
            dragStart = entt::null;
        }
        mouseButtonDown = true;
    }
    // mouse was pressed before
    if (mouseButtonDown && !(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
        if (reg.valid(dragStart)) {
            if (!reg.valid(hoverEntity)) {
                // drag ended in the world, create new anchor
                Services::level().createAnchor(x, y);
            }
            // TODO: create metal strut
        }
        mouseButtonDown = false;
    }
}
