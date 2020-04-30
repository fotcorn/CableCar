#include "Renderer.h"
#include "Texture.h"

#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
    // weird resolution to test aspect ratio and resolution independent rendering
    Renderer renderer(1700, 900);

    Texture::init(renderer.sdlRenderer());

    std::shared_ptr<Texture> texture = Texture::loadImage("level.png");

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

        texture->render();

        renderer.drawTexture(texture, 50, 50, 100, 100);

        renderer.flip();
    }

    return 0;
}
