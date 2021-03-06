#include "Input.h"

#include <iostream>

#include "Components.h"
#include "Level.h"
#include "Game.h"

bool Input::handleInput(entt::registry& reg) {
    entt::entity hoverEntity = handleMouse(reg);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return false;
            case SDL_KEYUP:
                handleKeyEvent(reg, event, hoverEntity);
                break;
        }
    }
    return true;
}

entt::entity Input::handleMouse(entt::registry& reg) {
    int x, y;
    Uint32 mouseState = SDL_GetMouseState(&x, &y);
    glm::vec2 mousePosition(x, y);

    // find current hovered anchor (if any)
    float minDistance = std::numeric_limits<float>::max();
    entt::entity hoverEntity = entt::null;

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
            Level::initBeamSprite(reg, tempDragBeam, start.position, mousePosition);
        } else {
            dragStart = entt::null;
        }
        mouseButtonDown = true;
    }
    // mouse was pressed before and is still pressed
    else if (mouseButtonDown && mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (reg.valid(dragStart)) {
            const Sprite& start = reg.get<Sprite>(dragStart);
            Level::updateBeamSprite(reg, tempDragBeam, start.position, mousePosition);
        }
    }
    // mouse was pressed before and is now released
    else if (mouseButtonDown && !(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
        if (reg.valid(dragStart)) {
            entt::entity dragEnd = hoverEntity;
            if (!reg.valid(dragEnd)) {
                // drag ended in the world, create new anchor
                dragEnd = Level::createAnchor(reg, static_cast<float>(x), static_cast<float>(y));
            }
            // create actual beam
            Level::createBeam(reg, dragStart, dragEnd);

            // destroy temporary beam
            reg.destroy(tempDragBeam);
        }
        mouseButtonDown = false;
    }

    return hoverEntity;
}

void Input::handleKeyEvent(entt::registry& reg, SDL_Event event, entt::entity hoverEntity) {
    SDL_Keycode key = event.key.keysym.sym;
    Uint16 mod = event.key.keysym.mod;

    bool noModKeys = !((KMOD_CTRL | KMOD_SHIFT | KMOD_ALT | KMOD_GUI) & mod);
    bool ctrlOnly = !((KMOD_SHIFT | KMOD_ALT | KMOD_GUI) & mod) && (mod & KMOD_CTRL);

    if (key == SDLK_s && ctrlOnly) {
        std::cout << "s" << std::endl;
    } else if (key == SDLK_r && noModKeys) {
        if (Game::get().gameMode() == Game::BUILD_MODE) {
            Game::get().setGameMode(Game::SIMULATION_MODE);
        } else {
            Game::get().setGameMode(Game::BUILD_MODE);
        }
    } else if (key == SDLK_ESCAPE && noModKeys) {
        if (Game::get().gameMode() == Game::SIMULATION_MODE) {
            Game::get().setGameMode(Game::BUILD_MODE);
        }
    } else if (key == SDLK_DELETE && noModKeys) {
        if (reg.valid(hoverEntity)) {
            Level::removeAnchor(reg, hoverEntity);
        }
    }
}
