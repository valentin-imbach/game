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
    bool stack = false;
    int count = 1;
    int type = 0;
    virtual void render(int x, int y, int scale, bool inv = true) {
        TextureManager::drawTexture(TextureManager::itemSheet, BIT*(type % 3), BIT*(type / 3), BIT, BIT, x, y, scale, scale,true);
    }
};

struct ItemStack : public Item {
    ItemStack(int t, int c = 1) {
        type = t;
        count = c;
        stack = true;
    }
    void render(int x, int y, int scale, bool inv = true) override {
        TextureManager::drawTexture(TextureManager::itemSheet, BIT*(type % 3), BIT*(type / 3), BIT, BIT, x, y, scale, scale,true);
        if (count != 1 && inv) {
            TextManager::drawText(std::to_string(count), x+scale/4, y);
        }
    }
};

struct Tool : public Item {
    int level = 0;
    Tool(int t) {
        type = t;
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

struct Loot {
    int min, max;
    int type;
    Loot(int t, int a = 1, int b = 1) {
        min = a;
        max = b;
        type = t;
    }
    Item* createItem() {
        return new ItemStack(type, min+(rand() % (max+1-min)));
    }
};

struct LootTable {
    v(Loot) table;
    LootTable() {}
    void addLoot(int t, int a = 1, int b = 1) {
        assert(a <= b);
        table.emplace_back(t,a,b);
    }
};
