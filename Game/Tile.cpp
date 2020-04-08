//
//  Tile.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Tile.hpp"

Tile::Tile() {}

Tile::Tile(int x, int y, TileID id) {
    textures = std::vector<std::pair<int,pint>>();
    pos = {x,y};
    tileID = id;
}

void Tile::update() {
    
}

void Tile::render() {
    for (auto t : textures) {
        Camera::placeTexture(TextureManager::tilesets[t.X],t.Y.X,t.Y.Y,1,1,pos);
    }
}
