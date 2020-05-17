#include <filesystem.hpp>

#include "AssetManager.h"

class FilesystemAssetManager : public AssetManager {
  public:
    FilesystemAssetManager(std::string directory);
    SDL_Surface* loadImage(std::string name);

  private:
    ghc::filesystem::path dataPath;
};
