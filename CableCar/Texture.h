#pragma once

#include <memory>
#include <string>
#include <unordered_map>

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;

class Texture {
  public:
    static std::shared_ptr<Texture> load(const std::string& path);
    ~Texture();
    int width() const;
    int height() const;

  private:
    explicit Texture(SDL_Texture* texture, const int width, const int height);

    SDL_Texture* m_texture;
    int m_width;
    int m_height;
    friend class Renderer;

    static std::unordered_map<std::string, std::shared_ptr<Texture>> cache;
};
