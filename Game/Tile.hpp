//
//  Tile.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "tools.h"
#include "Window.hpp"
#include "Camera.hpp"
#include "TextureManager.hpp"

struct Tile {
    Tile();
    Tile(int x, int y, TileID id);
    void render();
    void update();
    TileID tileID;
    pint pos;
    std::vector<std::pair<int,pint>> textures;
};
