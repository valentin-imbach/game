//
//  Tile.cpp
//  Game
//
//  Created by Valentin Imbach on 19/01/2021.
//  Copyright © 2021 Valentin Imbach. All rights reserved.
//

#include "Tile.hpp"
#include "TextureManager.hpp"

Tile::Tile(pair<int> pos, TileID tid) : position(pos), tileID(tid), neig() {}

void Tile::render() {
    for (auto s : sprites) Camera::drawSprite(s, position, true);
}

void Tile::addSprite(TileID tileID, pair<int> pos) {
    sprites.emplace_back(TextureManager::tilesets[tileID], pos, pair<int>(1,1), (int)tileID);
}

void Tile::updateStyle() {
    sprites.clear();
    addSprite(tileID, {1,1});
    for (int d = 0; d < 8; d += 2) {
        int l = neig[(d+2) % 8] -> tileID;
        int r = neig[(d+6) % 8] -> tileID;
        if (neig[d]  -> tileID < tileID && l != neig[d] -> tileID && r != neig[d] -> tileID)
            addSprite(neig[d] -> tileID, {1 - dirs2[d].X, 1 - dirs2[d].Y});
    }
    
    for (int d = 1; d < 8; d += 2) {
        int l = neig[(d+1) % 8] -> tileID;
        int r = neig[(d+7) % 8] -> tileID;
        if (neig[d] -> tileID < tileID && l != neig[d] -> tileID && r != neig[d] -> tileID)
            addSprite(neig[d] -> tileID, {1 - dirs2[d].X, 1 - dirs2[d].Y});
        if (l < tileID && l == r)
            addSprite(neig[d] -> tileID, {1 - dirs2[d].X, 4 - dirs2[d].Y});
    }
    std::sort(sprites.begin(), sprites.end(),[](const Sprite& a, const Sprite& b){ return a.priority > b.priority; });
}
