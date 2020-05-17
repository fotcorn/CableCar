#include "Texture.h"

#include <SDL_image.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "Services.h"

std::shared_ptr<Texture> Texture::loadImage(std::string path) {
    SDL_Surface* surface = Services::assetManager().loadImage(path);

    auto image = std::shared_ptr<Texture>(new Texture(surface->w, surface->h));
    image->texture = SDL_CreateTextureFromSurface(Services::renderer().sdlRenderer(), surface);
    if (image->texture == nullptr) {
        throw std::runtime_error(std::string("Failed to convert loaded image to texture: ") +
                                 std::string(SDL_GetError()));
    }

    SDL_FreeSurface(surface);
    return image;
}

Texture::~Texture() {
    SDL_DestroyTexture(texture);
}
