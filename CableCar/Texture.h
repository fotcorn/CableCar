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

    static void init(SDL_Renderer* pixelFormat);

  private:
    Texture() = default;
    SDL_Texture* texture = nullptr;
    static SDL_Renderer* renderer;
    static std::filesystem::path dataPath;
};
