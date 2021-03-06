//
//  TextureManager.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "TextureManager.hpp"
#include <SDL2_image/SDL_image.h>
#include "Window.hpp"

#include "Tile.hpp"

v(SDL_Texture*) TextureManager::tilesets = v(SDL_Texture*)(TileID::MAX, nullptr);
SDL_Texture* TextureManager::icons = nullptr;
SDL_Texture* TextureManager::icons2 = nullptr;
auto TextureManager::textures = std::map<std::string, SDL_Texture*>();

void TextureManager::Init() {
    refresh();
    LOG("TextureManager initialized");
}

SDL_Texture* TextureManager::getTexture(std::string path) {
    if (path == "") return nullptr;
    if (textures.find(path) != textures.end()) return textures[path];
    SDL_Texture* tex = loadTexture(path);
    textures[path] = tex;
    return tex;
}

SDL_Texture* TextureManager::loadTexture(std::string path) {
    std::string folder = "assets";
    SDL_Surface* tmpSurface = IMG_Load((folder + "/" + path).c_str());
    if (tmpSurface) LOG("Texture loaded from",path);
    else ERROR("Failed to load SDL_Texture from",path);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Window::renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    return tex;
};

void TextureManager::refresh() {
    loadTilesets();
    loadIcons();
    for (auto v : textures) {
        textures[v.first] = loadTexture(v.first);
    }
}

void TextureManager::drawTexture(SDL_Texture* tex, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh, bool centered, int alpha) {
    SDL_Rect src = {sx,sy,sw,sh};
    SDL_Rect dest = {dx,dy,dw,dh};
    if (centered) {
        dest.x -= dw/2;
        dest.y -= dh/2;
    }
    if (dest.x > Window::size.X+10 || dest.y > Window::size.Y+10 || dest.x+dest.w < -10 || dest.y+dest.h < -10) return;
    SDL_SetTextureAlphaMod(tex, alpha);
    SDL_RenderCopy(Window::renderer, tex, &src, &dest);
}

void TextureManager::drawTexture(SDL_Texture* tex, int dx, int dy, int dw, int dh, bool centered) {
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    drawTexture(tex, 0, 0, w, h, dx, dy, dw, dh, centered);
}

void TextureManager::drawTexture(SDL_Texture* tex, int dx, int dy, int scale, bool centered) {
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    drawTexture(tex, 0, 0, w, h, dx, dy, scale*w, scale*h, centered);
}

void TextureManager::drawTexture(SDL_Texture* tex, int dx, int dy, bool centered) {
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    drawTexture(tex, 0, 0, w, h, dx, dy, w, h, centered);
}

void TextureManager::drawRect(pair<int> pos, pair<int> size, int r, int g, int b) {
    SDL_Rect rect = {pos.X, pos.Y, size.X, size.Y};
    SDL_SetRenderDrawColor(Window::renderer, r, g, b, 255);
    SDL_RenderDrawRect(Window::renderer, &rect);
}

void TextureManager::loadTilesets() {
    tilesets[GRASS] = loadTexture("grass.png");
    tilesets[SAND] = loadTexture("sand.png");
    tilesets[WATER] = loadTexture("water.png");
    tilesets[STONE] = loadTexture("stone.png");
}

void TextureManager::loadIcons() {
    icons = loadTexture("icons.png");
    icons2 = loadTexture("icons2.png");
}
