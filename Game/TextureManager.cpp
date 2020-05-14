//
//  TextureManager.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "tools.h"
#include "TextureManager.hpp"
#include <SDL2_image/SDL_image.h>
#include "Window.hpp"

v(SDL_Texture*) TextureManager::tilesets = v(SDL_Texture*)(maxTileID,nullptr);
SDL_Texture* TextureManager::itemSheet = nullptr;
SDL_Texture* TextureManager::inventoryTexture = nullptr;
SDL_Texture* TextureManager::hotbarTexture = nullptr;
SDL_Texture* TextureManager::tableTexture = nullptr;

void TextureManager::Init() {
    
    loadTileset(GRASS,"assets/grass.png");
    loadTileset(SAND,"assets/sand.png");
    loadTileset(WATER,"assets/water.png");
    loadTileset(STONE,"assets/stone.png");
    
    itemSheet = loadTexture("assets/itemSheet.png");
    inventoryTexture = loadTexture("assets/inventory.png");
    hotbarTexture = loadTexture("assets/hotbar.png");
    tableTexture = loadTexture("assets/tableGui.png");
    
    LOG("TextureManager initialized");
}

SDL_Texture* TextureManager::loadTexture(const char* path) {
    SDL_Surface* tmpSurface = IMG_Load(path);
    if (tmpSurface) { LOG("Texture loaded from",path); } else { ERROR("Failed to load SDL_Texture from",path); }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Window::renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    return tex;
};

void TextureManager::drawTexture(SDL_Texture* tex, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh, bool centered) {
    SDL_Rect src = {sx,sy,sw,sh};
    SDL_Rect dest = {dx,dy,dw,dh};
    if (centered) { dest.x -= dw/2; dest.y -= dh/2; }
    SDL_RenderCopy(Window::renderer, tex, &src, &dest);
}

void TextureManager::drawTexture(SDL_Texture* tex, int dx, int dy, int dw, int dh, bool centered) {
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    SDL_Rect src = {0,0,w,h};
    SDL_Rect dest = {dx,dy,dw,dh};
    if (centered) { dest.x -= dw/2; dest.y -= dh/2; }
    SDL_RenderCopy(Window::renderer, tex, &src, &dest);
}

void TextureManager::drawTexture(SDL_Texture* tex, int dx, int dy, bool centered) {
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    SDL_Rect src = {0,0,w,h};
    SDL_Rect dest = {dx,dy,w,h};
    if (centered) { dest.x -= w/2; dest.y -= h/2; }
    SDL_RenderCopy(Window::renderer, tex, &src, &dest);
}

void TextureManager::drawRect(pair<int> pos, pair<int> size) {
    SDL_Rect rect = {pos.X, pos.Y, size.X, size.Y};
    SDL_RenderDrawRect(Window::renderer, &rect);
}

void TextureManager::loadTileset(int a, const char* path) {
    tilesets[a] = loadTexture(path);
}
