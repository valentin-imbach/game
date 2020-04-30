//
//  Map.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Map.hpp"

Map::Map(const char* path) {
    
    std::ifstream file;
    file.open(path);
    if (!file) {
        ERROR("Map file not found");
        file.close();
    }
    
    file >> size.X;
    file >> size.Y;
    
    tiles = vv(Tile)(size.X,v(Tile)(size.Y));
    
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
            tiles[x][y] = Tile(x,y,id);
        }
    }
    file.close();
    updateStyle();
    
    LOG("Map loaded from",path);
    
}

Map::Map(int w, int h) {
    size = {w,h};
    tiles = vv(Tile)(w,v(Tile)(h));
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
            if (noise.GetNoise(x*2,y*2) < -0.5) {
                tiles[x][y] = Tile(x,y,WATER);
            } else if (noise.GetNoise(x*2,y*2) < 0) {
                tiles[x][y] = Tile(x,y,STONE);
            } else if (noise.GetNoise(x*2,y*2) < 0.5) {
                tiles[x][y] = Tile(x,y,SAND);
            } else {
                tiles[x][y] = Tile(x,y,GRASS);
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
    for (int x = 1; x < size.X-1; x++) {
        for (int y = 1; y < size.Y-1; y++) {
            if (tiles[x-1][y].tileID == tiles[x+1][y].tileID) {
                tiles[x][y].tileID = tiles[x-1][y].tileID;
            }
            if (tiles[x][y-1].tileID == tiles[x][y+1].tileID) {
                tiles[x][y].tileID = tiles[x][y-1].tileID;
            }
        }
    }
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
            tiles[x][y].sprites.clear();
            tiles[x][y].sprites.push_back(Sprite(tiles[x][y].tileID,{1,1}));
            
            for (int d = 0; d < 8; d += 2) {
                int l = neig[(d+2) % 8];
                int r = neig[(d+6) % 8];
                if (neig[d] < tiles[x][y].tileID && l != neig[d] && r != neig[d]) {
                    pair<int> p = {1-dirs2[d].X,1-dirs2[d].Y};
                    tiles[x][y].sprites.push_back(Sprite(neig[d],p));
                }
            }
            
            for (int d = 1; d < 8; d += 2) {
                int l = neig[(d+1) % 8];
                int r = neig[(d+7) % 8];
                if (neig[d] < tiles[x][y].tileID && l != neig[d] && r != neig[d]) {
                    pair<int> p = {1-dirs2[d].X,1-dirs2[d].Y};
                    tiles[x][y].sprites.push_back(Sprite(neig[d],p));
                }
                if (l < tiles[x][y].tileID && l == r) {
                    pair<int> p = {1-dirs2[d].X,4-dirs2[d].Y};
                    tiles[x][y].sprites.push_back(Sprite(l,p));
                }
            }
            
            std::sort(tiles[x][y].sprites.begin(), tiles[x][y].sprites.end(),[](Sprite& a, Sprite&b){ return a.tileID > b.tileID; });
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





