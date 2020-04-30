//
//  TextureManager.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"

#define BIT 16

constexpr std::size_t maxTileID = 10;

enum TileID {
    GRASS,
    STONE,
    WATER,
    SAND
};

struct TextureManager {
    static void Init();
    static v(SDL_Texture*) tilesets;
    static SDL_Texture* itemSheet;
    static SDL_Texture* inventoryTexture;
    static SDL_Texture* hotbarTexture;
    static SDL_Texture* loadTexture(const char* path);
    static void loadTileset(int a, const char* path);
    static void drawTexture(SDL_Texture* tex, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh, bool centered = false);
    static void drawRect(pair<int> pos, pair<int> size);
};

struct Sprite {
    TileID tileID;
    SDL_Texture* texture;
    pair<int> pos;
    Sprite(int id, pair<int> p = {0,0}) {
        tileID = TileID(id);
        texture = TextureManager::tilesets[id];
        pos = p;
    }
};
