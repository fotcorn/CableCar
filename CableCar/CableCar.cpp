#include <SDL2/SDL.h>

#include "FilesystemAssetManager.h"
#include "Renderer.h"
#include "Services.h"
#include "Texture.h"

int main(int argc, char* argv[]) {
    Services services;

    Renderer renderer(1920, 1080);
    services.provideRenderer(&renderer);

    FilesystemAssetManager assetManager("data");
    services.provideAssetManager(&assetManager);

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

        renderer.drawTexture(texture, 0, 0, 1920, 1080);

        renderer.flip();
    }

    return 0;
}
