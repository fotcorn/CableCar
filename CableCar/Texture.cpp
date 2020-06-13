#include "Texture.h"

#include <SDL_image.h>

#include <stdexcept>

#include "Services.h"

Texture::Texture(SDL_Texture* texture, const int width, const int height)
    : m_texture(texture), m_width(width), m_height(height) {}

Texture::~Texture() {
    SDL_DestroyTexture(m_texture);
}

int Texture::height() const {
    return m_height;
}
int Texture::width() const {
    return m_width;
}
