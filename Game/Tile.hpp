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

class Tile {
public:
    Tile();
    Tile(int x, int y, std::string t);
    void render();
    void update();
    void updateStyle(int neig[8]);
    inline int getId() { return id; }
    static v(std::string) types;
    static std::map<std::string,int> priority;
    static void loadTypes();
private:
    std::string type;
    int id;
    pint pos;
    std::vector<std::pair<int,pint>> textures;
    v(pint) positions;
};
