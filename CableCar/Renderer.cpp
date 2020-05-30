#include "Renderer.h"

#include <stdexcept>
#include <string>

#include "Services.h"

Renderer::Renderer(int screenWidth, int screenHeight) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(std::string("Failed to initialize video: ") + SDL_GetError());
    }

    window = SDL_CreateWindow("CableCar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        throw std::runtime_error(std::string("Failed to create window: ") + SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        throw std::runtime_error(std::string("Failed to create renderer: ") + SDL_GetError());
    }

    // based on http://www.david-amador.com/2013/04/opengl-2d-independent-resolution-rendering/
    viewportWidth = screenWidth;
    viewportHeight = static_cast<int>(viewportWidth / VIRTUAL_ASPECT_RATIO + 0.5f);

    if (viewportHeight > screenHeight) {
        viewportHeight = screenHeight;
        viewportWidth = static_cast<int>(viewportHeight * VIRTUAL_ASPECT_RATIO + 0.5f);
    }

    viewportToVirtualX = static_cast<float>(viewportWidth) / static_cast<float>(VIRTUAL_WIDTH);
    viewportToVirtualY = static_cast<float>(viewportHeight) / static_cast<float>(VIRTUAL_HEIGHT);

    int viewportX = (screenWidth / 2) - (viewportWidth / 2);
    int viewportY = (screenHeight / 2) - (viewportHeight / 2);

    SDL_Rect viewport;
    viewport.x = viewportX;
    viewport.y = viewportY;
    viewport.w = viewportWidth;
    viewport.h = viewportHeight;
    SDL_RenderSetViewport(renderer, &viewport);
}

Renderer::~Renderer() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::clear() {
    SDL_RenderClear(renderer);
}

void Renderer::flip() {
    SDL_RenderPresent(renderer);
}

void Renderer::drawTexture(const Texture& texture, const Transform& transform) {
    float xPosition = transform.position.x - transform.origin.x;
    float yPosition = transform.position.y - transform.origin.y;

    SDL_Rect destinationRect;
    destinationRect.x = xPosition * viewportToVirtualX;
    destinationRect.y = yPosition * viewportToVirtualY;
    destinationRect.w = transform.dimensions.x * viewportToVirtualX;
    destinationRect.h = transform.dimensions.y * viewportToVirtualY;

    SDL_RenderCopy(renderer, texture.m_texture, nullptr, &destinationRect);
}

void Renderer::render() {
    entt::registry& reg = Services::registry();

    clear();

    float minDistance = std::numeric_limits<float>::max();

    entt::entity hoverEntity = entt::null;
    auto view = reg.view<CurrentHover>();
    if (const auto it = view.begin(); it != view.end()) {
        hoverEntity = *it;
    }

    reg.view<Sprite, Transform>().each([hoverEntity, this](auto entity, auto& sprite, auto& transform) {
        if (hoverEntity == entity) {
            return;
        }
        drawTexture(*sprite.texture.get(), transform);
    });

    if (reg.valid(hoverEntity)) {
        auto [transform, hoverTarget] = reg.get<Transform, HoverTarget>(hoverEntity);
        if (hoverTarget.hoverTexture) {
            drawTexture(*hoverTarget.hoverTexture.get(), transform);
        }
    }

    flip();
}
