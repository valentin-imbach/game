//
//  Map.cpp
//  Game
//
//  Created by Valentin Imbach on 19/01/2021.
//  Copyright © 2021 Valentin Imbach. All rights reserved.
//

#include "Map.hpp"

Map::Map(pair<int> size) : size(size) {
    tiles = vv(Tile*)(size.X,v(Tile*)(size.Y));
}

void Map::render() {
    for (int x = 0; x < size.X; x++) {
        for (int y = 0; y < size.Y; y++) {
            tiles[x][y] -> render();
        }
    }
}

Map* Map::load(const char* path) {
    std::ifstream file;
    file.open(path);
    if (!file) {
        ERROR("Map file not found");
        file.close();
        return nullptr;
    }
    
    pair<int> size;
    file >> size.X;
    file >> size.Y;
    
    Map* map = new Map(size);

    for (int y = 0; y < size.Y; y++) {
        std::string s;
        file >> s;
        for (int x = 0; x < size.X; x++) {
            TileID id = WATER;
            switch (s[x]) {
                case '0':
                    id = WATER;
                    break;
                case '1':
                    id = SAND;
                    break;
                case '2':
                    id = STONE;
                    break;
                case '3':
                    id = GRASS;
                    break;
            }
            map -> tiles[x][y] = new Tile({x,y},id);
        }
    }
    
    file.close();
    
    for (int y = 1; y < size.Y-1; y++) {
        for (int x = 1; x < size.X-1; x++) {
            map -> tiles[x][y] -> neig[0] = map -> tiles[x+1][y];
            map -> tiles[x][y] -> neig[1] = map -> tiles[x+1][y-1];
            map -> tiles[x][y] -> neig[2] = map -> tiles[x][y-1];
            map -> tiles[x][y] -> neig[3] = map -> tiles[x-1][y-1];
            map -> tiles[x][y] -> neig[4] = map -> tiles[x-1][y];
            map -> tiles[x][y] -> neig[5] = map -> tiles[x-1][y+1];
            map -> tiles[x][y] -> neig[6] = map -> tiles[x][y+1];
            map -> tiles[x][y] -> neig[7] = map -> tiles[x+1][y+1];
            map -> tiles[x][y] -> updateStyle();
        }
    }
    
    LOG("Map loaded from", path);
    
    return map;
}
