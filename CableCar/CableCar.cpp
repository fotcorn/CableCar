#include "Renderer.h"
#include "Texture.h"

#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
    // weird resolution to test aspect ratio and resolution independent rendering
    Renderer renderer(1700, 900);

    Texture::init(renderer.sdlRenderer());

    std::shared_ptr<Texture> texture = Texture::loadImage("level.png");
    float textureAspectRatio = static_cast<float>(texture->height) / static_cast<float>(texture->width);

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

        renderer.clear();

        renderer.drawTexture(texture, 50, 50, 100, static_cast<int>(100 * textureAspectRatio));

        renderer.flip();
    }

    return 0;
}
