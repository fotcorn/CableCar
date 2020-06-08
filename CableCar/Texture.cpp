#include "Texture.h"

#include <SDL_image.h>

#include <stdexcept>

#include "Services.h"

std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::cache;

Texture::Texture(SDL_Texture* texture, const int width, const int height)
    : m_texture(texture), m_width(width), m_height(height) {}

Texture::~Texture() {
    SDL_DestroyTexture(m_texture);
}

std::shared_ptr<Texture> Texture::load(const std::string& path) {
    auto it = cache.find(path);
    if (it != cache.end()) {
        return it->second;
    }

    SDL_Surface* surface = Services::assetManager().loadImage(path);
    int width = surface->w;
    int height = surface->h;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Services::renderer().sdlRenderer(), surface);
    if (texture == nullptr) {
        throw std::runtime_error(std::string("Failed to convert loaded image to texture: ") +
                                 std::string(SDL_GetError()));
    }
    SDL_FreeSurface(surface);

    auto ptr = std::shared_ptr<Texture>(new Texture(texture, width, height));
    cache[path] = ptr;
    return ptr;
}

int Texture::height() const {
    return m_height;
}
int Texture::width() const {
    return m_width;
}
