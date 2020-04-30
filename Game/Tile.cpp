//
//  Tile.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Tile.hpp"
#include "Camera.hpp"

Tile::Tile() {}

Tile::Tile(int x, int y, TileID id) {
    pos = {x,y};
    tileID = id;
}

void Tile::update() {
    
}

void Tile::render() {
    for (auto s : sprites) {
        Camera::drawTexture(s.texture, s.pos.X, s.pos.Y, 1, 1, pos, {1, 1}, 0, true);
    }
}
