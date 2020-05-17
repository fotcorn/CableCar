#pragma once

#include <memory>
#include <string>

struct SDL_Renderer;
struct SDL_Texture;

class Texture {
  public:
    static std::shared_ptr<Texture> loadImage(std::string path);
    ~Texture();
    const int width;
    const int height;

  private:
    Texture(int width, int height) : width(width), height(height){};
    SDL_Texture* texture = nullptr;
    friend class Renderer;
};
