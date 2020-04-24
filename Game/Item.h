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
    int type;
    int count;
    virtual void render(int x, int y, int scale) {};
};

class ItemStack : public Item {
public:
    ItemStack(int t, int c) {
        type = t;
        count = c;
        stack = true;
    }
    void render(int x, int y, int scale) override {
        TextureManager::drawTexture(TextureManager::itemSheet, TILE_SIZE*type, 0, TILE_SIZE, TILE_SIZE, x, y, scale, scale);
        if (count > 1) {
            TextManager::drawText(std::to_string(count), x+3*scale/4, y+scale/2);
        }
    }
};
