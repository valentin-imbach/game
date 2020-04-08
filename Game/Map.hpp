//
//  Map.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#include "Tile.hpp"
#include "Resource.hpp"
#include "tools.h"
#include "FastNoise.h"
#include "ECS/ECS.h"

class Map {
public:
    Map(int w, int h);
    void render();
    void update();
    bool check(int x, int y);
    inline bool hasSolid(int x, int y) { return check(x,y) ? entities[x][y] != nullptr : false; }
    vv(Entity*) entities;
private:
    void updateStyle();
    void generate();
    pint size;
    vv(Tile) tiles;
};

