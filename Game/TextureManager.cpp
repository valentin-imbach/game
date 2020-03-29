//
//  TextureManager.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "TextureManager.hpp"

auto TextureManager::tilesets = std::map<std::string,SDL_Texture*>();
SDL_Texture* TextureManager::character = nullptr;
SDL_Texture* TextureManager::spriteSheet = nullptr;
SDL_Texture* TextureManager::itemSheet = nullptr;
SDL_Texture* TextureManager::hotbar = nullptr;

void TextureManager::Init() {
    loadTileset("grass", "assets/grass.png");
    loadTileset("sand", "assets/sand.png");
}

SDL_Texture* TextureManager::loadTexture(const char* path) {
    SDL_Surface* tmpSurface = IMG_Load(path);
    if (tmpSurface) { LOG("Texture loaded from " << path); } else { ERROR("Failed to load SDL_Texture from " << path); }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Window::renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    return tex;
};

void TextureManager::drawTexture(SDL_Texture* tex, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh) {
    SDL_Rect src;
    src.x = sx; src.y = sy; src.w = sw; src.h = sh;
    SDL_Rect dest;
    dest.x = dx; dest.y = dy; dest.w = dw; dest.h = dh;
    SDL_RenderCopy(Window::renderer, tex, &src, &dest);
}

void TextureManager::loadTileset(std::string name, const char* path) {
    tilesets[name] = loadTexture(path);
}
