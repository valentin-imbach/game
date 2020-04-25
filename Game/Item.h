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

struct Item {
    Item() {
        //PRINT("Item constructed");
    }
    bool stack = false;
    int type;
    int count;
    virtual void render(int x, int y, int scale) {};
    ~Item() {
        //PRINT("Item destructed");
    }
};

struct ItemStack : public Item {
    ItemStack(int t, int c) {
        type = t;
        count = c;
        stack = true;
    }
    void render(int x, int y, int scale) override {
        TextureManager::drawTexture(TextureManager::itemSheet, TILE_SIZE*type, 0, TILE_SIZE, TILE_SIZE, x, y, scale, scale);
        if (count != 1) {
            TextManager::drawText(std::to_string(count), x+3*scale/4, y+scale/2);
        }
    }
};

struct ItemSlot {
    
    Item* item = nullptr;
    
    Item* addFullItem(Item* other) {
        
        if (item == nullptr) {
            item = other;
            return nullptr;
        }
        
        if (similar(other)) {
            item -> count += other -> count;
            delete other;
            return nullptr;
        }
        
        return other;
    }
    
    Item* addOneItem(Item* other) {
        
        if (item == nullptr) {
            if (other == nullptr) {
                return nullptr;
            }
            if (other -> stack == false) {
                item = other;
                return nullptr;
            }
            item = new ItemStack(other -> type,1);
            other -> count -= 1;
            if (other -> count == 0) {
                delete other;
                return nullptr;
            }
            return other;
        }
        
        if (similar(other)) {
            item -> count += 1;
            other -> count -= 1;
            if (other -> count == 0) {
                delete other;
                return nullptr;
            }
            return other;
        }
        return other;
    }
    
    Item* takeHalf() {
        if (item == nullptr || item -> stack == false || item -> count == 1) {
            Item* temp = item;
            item = nullptr;
            return temp;
        }
        int count = (item -> count + 1) / 2;
        item -> count -= count;
        return new ItemStack(item -> type, count);
    }
    
    bool similar(Item* other) {
        return (item != nullptr && other != nullptr && item -> stack && other -> stack && (item -> type == other -> type));
    }
    
};
