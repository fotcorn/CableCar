#include <filesystem.hpp>

#include "AssetManager.h"

class FilesystemAssetManager : public AssetManager {
  public:
    FilesystemAssetManager(std::string directory, SDL_Renderer* sdlRenderer);
    std::shared_ptr<SDL_Surface> loadImage(std::string name) override;
    std::shared_ptr<Texture> loadTexture(std::string name) override;

  private:
    ghc::filesystem::path dataPath;
    SDL_Renderer* sdlRenderer;
};
