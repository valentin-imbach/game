//
//  Tile.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Tile.hpp"

v(std::string) Tile::types = v(std::string)();
auto Tile::priority = std::map<std::string,int>();

void Tile::loadTypes() {
    
    types.push_back("grass");
    types.push_back("sand");
    
    for (int i = 0; i < types.size(); i++) {
        priority[types[i]] = i;
    }
}

Tile::Tile() {}

Tile::Tile(int x, int y, std::string t) {
    textures = std::vector<std::pair<int,pint>>();
    pos = {x,y};
    type = t;
    id = priority[t];
}

void Tile::update() {
    
}

void Tile::updateStyle(int neig[8]) {
    
    textures.clear();
    textures.push_back({id,{1,1}});
    
    for (int d = 0; d < 8; d += 2) {
        int l = neig[(d+2) % 8];
        int r = neig[(d+6) % 8];
        if (neig[d] < id && l != neig[d] && r != neig[d]) {
            pint p = {1-dirs2[d].X,1-dirs2[d].Y};
            textures.emplace_back(neig[d],p);
        }
    }
    
    for (int d = 1; d < 8; d += 2) {
        int l = neig[(d+1) % 8];
        int r = neig[(d+7) % 8];
        if (neig[d] < id && l != neig[d] && r != neig[d]) {
            pint p = {1-dirs2[d].X,1-dirs2[d].Y};
            textures.emplace_back(neig[d],p);
        }
        if (l < id && l == r) {
            pint p = {1-dirs2[d].X,4-dirs2[d].Y};
            textures.emplace_back(l,p);
        }
    }

    std::sort(textures.rbegin(), textures.rend());
}

void Tile::render() {
    for (auto t : textures) {
        Camera::placeTexture(TextureManager::tilesets[types[t.X]],t.Y.X,t.Y.Y,1,1,pos);
    }
}
