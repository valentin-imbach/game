//
//  ItemStack.cpp
//  Game
//
//  Created by Valentin Imbach on 17.12.19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "ItemStack.hpp"
#include "TextureManager.hpp"

ItemStack::ItemStack() {}

ItemStack::ItemStack(int t, int c) {
    type = t;
    count = c;
}

void ItemStack::render(int x, int y, float scale = 1) {
    TextureManager::drawTexture(TextureManager::itemSheet, TILE_SIZE*type, 0, TILE_SIZE, TILE_SIZE, x, y, scale*TILE_SIZE, scale*TILE_SIZE);
}

