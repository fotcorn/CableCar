#include "Texture.h"

#include <SDL_image.h>

#include <stdexcept>

#include "Services.h"

Texture::Texture(const std::string& path) {
    SDL_Surface* surface = Services::assetManager().loadImage(path);
    init(surface);
    SDL_FreeSurface(surface);
}
Texture::Texture(SDL_Surface* surface) {
    init(surface);
}

Texture::~Texture() {
    SDL_DestroyTexture(m_texture);
}

void Texture::init(SDL_Surface* surface) {
    m_width = surface->w;
    m_height = surface->h;
    m_texture = SDL_CreateTextureFromSurface(Services::renderer().sdlRenderer(), surface);
    if (m_texture == nullptr) {
        throw std::runtime_error(std::string("Failed to convert loaded image to texture: ") +
                                 std::string(SDL_GetError()));
    }
}

int Texture::height() const {
    return m_height;
}
int Texture::width() const {
    return m_width;
}
