#pragma once

#include <memory>
#include <string>

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;

class Texture {
  public:
    explicit Texture(const std::string& path);
    explicit Texture(SDL_Surface* surface);
    ~Texture();
    int width() const;
    int height() const;

  private:
    void init(SDL_Surface* surface);

    SDL_Texture* m_texture = nullptr;
    int m_width;
    int m_height;
    friend class Renderer;
};
