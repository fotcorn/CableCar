#pragma once

#include <filesystem>
#include <memory>
#include <string>

struct SDL_Renderer;
struct SDL_Texture;

class Texture {
  public:
    static std::shared_ptr<Texture> loadImage(std::string path);
    ~Texture();

    void render();
    const int width;
    const int height;

    static void init(SDL_Renderer* pixelFormat);

  private:
    Texture(int width, int height) : width(width), height(height){};
    SDL_Texture* texture = nullptr;
    static SDL_Renderer* renderer;
    static std::filesystem::path dataPath;
};
