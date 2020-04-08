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
                tiles[x][y] = Tile(x,y,GRASS);
            } else {
                tiles[x][y] = Tile(x,y,SAND);
            }
        }
    }
    updateStyle();
}

void Map::render() {
    for (int x = 0; x < size.X; x++) {
        for (int y = 0; y < size.Y; y++) {
            tiles[x][y].render();
        }
    }
}

void Map::updateStyle() {
    for (int x = 0; x < size.X; x++) {
        for (int y = 0; y < size.Y; y++) {
            int neig[8];
            for (int i = 0; i < 8; i++) {
                if (check(x+dirs2[i].X, y+dirs2[i].Y)) {
                    neig[i] = tiles[x+dirs2[i].X][y+dirs2[i].Y].tileID;
                } else {
                    neig[i] = tiles[x][y].tileID;
                }
            }
            tiles[x][y].textures.clear();
            tiles[x][y].textures.push_back({tiles[x][y].tileID,{1,1}});
            
            for (int d = 0; d < 8; d += 2) {
                int l = neig[(d+2) % 8];
                int r = neig[(d+6) % 8];
                if (neig[d] < tiles[x][y].tileID && l != neig[d] && r != neig[d]) {
                    pint p = {1-dirs2[d].X,1-dirs2[d].Y};
                    tiles[x][y].textures.emplace_back(neig[d],p);
                }
            }
            
            for (int d = 1; d < 8; d += 2) {
                int l = neig[(d+1) % 8];
                int r = neig[(d+7) % 8];
                if (neig[d] < tiles[x][y].tileID && l != neig[d] && r != neig[d]) {
                    pint p = {1-dirs2[d].X,1-dirs2[d].Y};
                    tiles[x][y].textures.emplace_back(neig[d],p);
                }
                if (l < tiles[x][y].tileID && l == r) {
                    pint p = {1-dirs2[d].X,4-dirs2[d].Y};
                    tiles[x][y].textures.emplace_back(l,p);
                }
            }
            
            std::sort(tiles[x][y].textures.rbegin(), tiles[x][y].textures.rend());
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





