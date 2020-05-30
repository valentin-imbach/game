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
#include "ECS/ECS.h"

#define MAX_STACK 5

struct Item {
    bool stack = false;
    int count = 1;
    int type = 0;
    virtual void render(int x, int y, int scale, bool inv = true) {
        TextureManager::drawTexture(TextureManager::getTexture("itemSheet.png"), BIT*(type % 3), BIT*(type / 3), BIT, BIT, x, y, scale, scale, true);
    }
    virtual bool onClick(int b) { return false; }
};

struct ItemStack : public Item {
    ItemStack(int t, int c = 1) {
        type = t;
        count = c;
        stack = true;
    }
    void render(int x, int y, int scale, bool inv = true) override {
        TextureManager::drawTexture(TextureManager::getTexture("itemSheet.png"), BIT*(type % 3), BIT*(type / 3), BIT, BIT, x, y, scale, scale, true);
        if (count != 1 && inv) {
            TextManager::drawText(std::to_string(count), {x+scale/4,y});
        }
    }
};

struct ItemContainer {
    
    Item* item = nullptr;
    
    bool empty() { return item == nullptr; }
    bool stack() { return item ? item -> stack : false; }
    int type() { return item ? item -> type : -1; }
    int count() { return item -> count; }
    
    bool similar(Item* other) {
        return (item != nullptr && other != nullptr && item -> stack && other -> stack && (item -> type == other -> type));
    }
    
    bool similar(ItemContainer* other) {
        return (!empty() && !other -> empty() && stack() && other -> stack() && type() == other -> type());
    }
    
    bool takeHalf(ItemContainer* other) {
        if (!other -> empty()) { return false; }
        if (empty() || !stack() || count() == 1) {
            other -> item = item;
            item = nullptr;
            return true;
        }
        int c = (count() + 1) / 2;
        item -> count -= c;
        other -> item = new ItemStack(item -> type, c);
        return true;
    }
    
    bool addFull(ItemContainer* other) {
        if (similar(other)) {
            item -> count += other -> item -> count;
            other -> item = nullptr;
            return true;
        }
        
        if (empty()) {
            item = other -> item;
            other -> item = nullptr;
            return true;
        }
        return false;
    }
    
    bool addOne(ItemContainer* other) {
        if (empty()) {
            if (other -> empty()) {
                return false;
            }
            if (!other -> stack()) {
                item = other -> item;
                other -> item = nullptr;
                return true;
            }
            item = new ItemStack(other -> type(),1);
            other -> item -> count -= 1;
            if (other -> item -> count == 0) {
                other -> item = nullptr;
            }
            return true;
        }
        
        if (similar(other)) {
            item -> count += 1;
            other -> item -> count -= 1;
            if (other -> item -> count == 0) {
                other -> item = nullptr;
            }
            return true;
        }
        return false;
    }
    
    void render(pair<int> pos, int scale) {
        if (item != nullptr) {
            item -> render(pos.X, pos.Y, scale);
        }
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

class Tool : public Item {
public:
    int level = 0;
    Tool(int t) {
        type = t;
    }
};

class Consumable : public ItemStack {
public:
    Consumable(int t, int c) : ItemStack(t,c) {};
    bool onClick(int b) override {
        if (b == SDL_BUTTON_RIGHT) {
            count -= 1;
            LOG("Item of type",type,"consumed");
        }
        return true;
    }
};
