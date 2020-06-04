#include "Input.h"

#include <iostream>

#include "Components.h"
#include "Services.h"

bool Input::handleInput() {
    entt::entity hoverEntity = handleMouse();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return false;
            case SDL_KEYUP:
                handleKeyEvent(event, hoverEntity);
                break;
        }
    }
    return true;
}

entt::entity Input::handleMouse() {
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
            entt::entity dragEnd = hoverEntity;
            if (!reg.valid(dragEnd)) {
                // drag ended in the world, create new anchor
                dragEnd = Services::level().createAnchor(x, y);
            }
            Services::level().createBeam(dragStart, dragEnd);
        }
        mouseButtonDown = false;
    }

    return hoverEntity;
}

void Input::handleKeyEvent(SDL_Event event, entt::entity hoverEntity) {
    SDL_Keycode key = event.key.keysym.sym;
    Uint16 mod = event.key.keysym.mod;

    bool noModKeys = !((KMOD_CTRL | KMOD_SHIFT | KMOD_ALT | KMOD_GUI) & mod);
    bool ctrlOnly = !((KMOD_SHIFT | KMOD_ALT | KMOD_GUI) & mod) && (mod & KMOD_CTRL);

    if (key == SDLK_s && ctrlOnly) {
        std::cout << "s" << std::endl;
    }
    if (key == SDLK_r && ctrlOnly) {
        std::cout << "r" << std::endl;
    }
    if (key == SDLK_DELETE && noModKeys) {
        std::cout << "delete " << std::endl;
    }
}
