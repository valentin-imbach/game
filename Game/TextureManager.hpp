//
//  TextureManager.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#define TILE_SIZE 16

#include "Window.hpp"
#include <SDL2_image/SDL_image.h>

constexpr std::size_t maxTileID = 10;

enum TileID {
    GRASS,
    STONE,
    WATER,
    SAND
};

struct TextureManager {
    static void Init();
    static std::vector<SDL_Texture*> tilesets;
    static SDL_Texture* character;
    static SDL_Texture* spriteSheet;
    static SDL_Texture* itemSheet;
    static SDL_Texture* hotbar;
    static SDL_Texture* loadTexture(const char* path);
    static void loadTileset(int a, const char* path);
    static void drawTexture(SDL_Texture* tex, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh);
};
