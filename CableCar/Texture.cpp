#pragma once

#include "Texture.h"

#include <SDL_image.h>

#include <cassert>
#include <memory>
#include <stdexcept>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void Texture::init(SDL_Renderer* renderer) {
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        throw std::runtime_error(
            "Failed to initialize SDL_image with png support");
    }
    Texture::renderer = renderer;

    fs::path directory = fs::current_path();
    bool found = false;
    while (!found) {
        for (auto entry : fs::directory_iterator(directory)) {
            if (entry.is_directory() && entry.path().filename() == "data") {
                Texture::dataPath = entry.path();
                found = true;
            }
        }

        directory = directory.parent_path();
        if (!directory.has_relative_path()) {
            throw std::runtime_error("Could not find data directory");
        }
    }
}

std::shared_ptr<Texture> Texture::loadImage(std::string path) {
    assert(Texture::renderer != nullptr);

    auto image = std::shared_ptr<Texture>(new Texture());

    fs::path fsPath = Texture::dataPath / path;
    if (!fs::exists(fsPath)) {
        throw std::runtime_error(std::string("Failed to find image file: ") +
                                 fsPath.string());
    }

    SDL_Surface* surface = IMG_Load(fsPath.string().c_str());
    if (surface == nullptr) {
        throw std::runtime_error(std::string("Failed to load image:") +
                                 std::string(IMG_GetError()));
    }

    image->texture = SDL_CreateTextureFromSurface(Texture::renderer, surface);
    if (image->texture == nullptr) {
        throw std::runtime_error(
            std::string("Failed to convert loaded image to texture: ") +
            std::string(SDL_GetError()));
    }

    SDL_FreeSurface(surface);
    return image;
}

void Texture::render() {
    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

Texture::~Texture() {
    SDL_DestroyTexture(texture);
}

SDL_Renderer* Texture::renderer = nullptr;
fs::path Texture::dataPath = fs::path();
