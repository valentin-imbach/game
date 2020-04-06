//
//  Map.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Map.hpp"

Map::Map(int w, int h) {
    size = {w,h};
    tiles = vv(Tile)(w,v(Tile)(h));
    objects = vv(GameObject*)(w,v(GameObject*)(h,nullptr));
    entities = vv(Entity*)(w,v(Entity*)(h,nullptr));
    generate();
}

bool Map::check(int x, int y) {
    return (0 <= x && x < size.X && 0 <= y && y < size.Y);
}

void Map::generate() {
    FastNoise noise;
    noise.SetNoiseType(FastNoise::SimplexFractal);
    noise.SetSeed(rand());
    for (int x = 0; x < size.X; x++) {
        for (int y = 0; y < size.Y; y++) {
            if (noise.GetNoise(x*2,y*2) < 0) {
                tiles[x][y] = Tile(x,y,"grass");
            } else {
                tiles[x][y] = Tile(x,y,"sand");
            }
        }
    }
    for (int x = 0; x < size.X; x++) {
        for (int y = 0; y < size.Y; y++) {
            int neig[8];
            for (int i = 0; i < 8; i++) {
                if (check(x+dirs2[i].X, y+dirs2[i].Y)) {
                    neig[i] = tiles[x+dirs2[i].X][y+dirs2[i].Y].getId();
                } else {
                    neig[i] = tiles[x][y].getId();
                }
            }
            tiles[x][y].updateStyle(neig);
        }
    }
    
    objects[1][1] = new Resource(1,1,1,4,3);
    objects[3][1] = new Resource(3,1,1,4,3);
    
}

void Map::render() {
    for (int x = 0; x < size.X; x++) {
        for (int y = 0; y < size.Y; y++) {
            tiles[x][y].render();
        }
    }
    for (int y = 0; y < size.Y; y++) {
        for (int x = 0; x < size.X; x++) {
            if (objects[x][y] == nullptr) {
                continue;
            }
            objects[x][y] -> render();
        }
    }
}

void Map::update() {
    for (int x = 0; x < size.X; x++) {
        for (int y = 0; y < size.Y; y++) {
            tiles[x][y].update();
        }
    }
}





