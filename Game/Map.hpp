//
//  Map.hpp
//  Game
//
//  Created by Valentin Imbach on 19/01/2021.
//  Copyright © 2021 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "Tile.hpp"

class Map {
private:
    pair<int> size;
    vv(Tile*) tiles;
public:
    Map(pair<int> size);
    static Map* load(const char* path);
    
    void update() {}
    void render();
    bool handleEvent(SDL_Event event) { return false; }
};
