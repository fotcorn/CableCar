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

            tempDragBeam = reg.create();
            // we do not actually add a Beam component to the temporary entity,
            // a temporary beam is not really part of a level
            const Sprite& start = reg.get<Sprite>(dragStart);
            Services::level().initBeamSprite(tempDragBeam, start.position, mousePosition);
        } else {
            dragStart = entt::null;
        }
        mouseButtonDown = true;
    }
    // mouse was pressed before and is still pressed
    else if (mouseButtonDown && mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (reg.valid(dragStart)) {
            const Sprite& start = reg.get<Sprite>(dragStart);
            Services::level().updateBeamSprite(tempDragBeam, start.position, mousePosition);
        }
    }
    // mouse was pressed before and is now released
    else if (mouseButtonDown && !(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
        if (reg.valid(dragStart)) {
            entt::entity dragEnd = hoverEntity;
            if (!reg.valid(dragEnd)) {
                // drag ended in the world, create new anchor
                dragEnd = Services::level().createAnchor(x, y);
            }
            // create actual beam
            Services::level().createBeam(dragStart, dragEnd);

            // destroy temporary beam
            reg.destroy(tempDragBeam);
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
    } else if (key == SDLK_r && ctrlOnly) {
        if (Services::level().gameMode() == Level::BUILD_MODE) {
            Services::level().setGameMode(Level::SIMULATION_MODE);
        } else {
            Services::level().setGameMode(Level::BUILD_MODE);
        }
    } else if (key == SDLK_ESCAPE && noModKeys) {
        if (Services::level().gameMode() == Level::SIMULATION_MODE) {
            Services::level().setGameMode(Level::BUILD_MODE);
        }
    } else if (key == SDLK_DELETE && noModKeys) {
        if (Services::registry().valid(hoverEntity)) {
            Services::level().removeAnchor(hoverEntity);
        }
    }
}
