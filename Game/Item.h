//
//  Item.h
//  Game
//
//  Created by Valentin Imbach on 13.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "TextureManager.hpp"
#include "TextManager.hpp"

#define MAX_STACK 5

class Item {
public:
    bool stack = false;
    virtual void render(int x, int y, int scale) {};
};

class ItemStack : public Item {
private:
    int type;
    int count;
public:
    ItemStack(int t, int c) {
        type = t;
        count = c;
        stack = true;
    }
    void render(int x, int y, int scale) override {
        TextureManager::drawTexture(TextureManager::itemSheet, TILE_SIZE*type, 0, TILE_SIZE, TILE_SIZE, x, y, scale, scale);
        TextManager::drawText(std::to_string(count), x+scale, y+scale/2);
    }
};
