#pragma once

#include <memory>
#include <string>
#include <unordered_map>

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;

class Texture {
  public:
    explicit Texture(SDL_Texture* texture, const int width, const int height);
    ~Texture();
    int width() const;
    int height() const;

    SDL_Texture* m_texture;
    int m_width;
    int m_height;
};
