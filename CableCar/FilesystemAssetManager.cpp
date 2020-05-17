#include "FilesystemAssetManager.h"

namespace fs = ghc::filesystem;

FilesystemAssetManager::FilesystemAssetManager(std::string directory) {
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        throw std::runtime_error("Failed to initialize SDL_image with png support");
    }

    fs::path dir = fs::current_path();
    bool found = false;
    while (!found) {
        for (auto entry : fs::directory_iterator(dir)) {
            if (entry.is_directory() && entry.path().filename() == directory) {
                dataPath = entry.path();
                found = true;
            }
        }

        dir = dir.parent_path();
        if (!dir.has_relative_path()) {
            // we reached the root directory without finding the directory
            throw std::runtime_error("Could not find asset directory");
        }
    }
}

SDL_Surface* FilesystemAssetManager::loadImage(std::string name) {
    fs::path fsPath = dataPath / name;
    if (!fs::exists(fsPath)) {
        throw std::runtime_error(std::string("Failed to find image file: ") + fsPath.string());
    }

    SDL_Surface* surface = IMG_Load(fsPath.string().c_str());
    if (surface == nullptr) {
        throw std::runtime_error(std::string("Failed to load image:") + std::string(IMG_GetError()));
    }
    return surface;
}
