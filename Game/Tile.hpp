//
//  Tile.hpp
//  Game
//
//  Created by Valentin Imbach on 19/01/2021.
//  Copyright © 2021 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "Camera.hpp"

enum TileID {
    NONE = -1,
    GRASS,
    DIRT,
    STONE,
    WATER,
    SAND,
    
    MAX
};

class Tile {
private:
    v(Sprite) sprites;
    void addSprite(TileID tileID, pair<int> pos);
public:
    Tile* neig[8];
    pair<int> position;
    TileID tileID;

    Tile(pair<int> pos, TileID tid = TileID::NONE);
    void render();
    void updateStyle();
    
};


