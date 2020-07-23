//
//  Item.h
//  Game
//
//  Created by Valentin Imbach on 13.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "item.h"

#include "TextureManager.hpp"
#include "TextManager.hpp"
#include "ECS/ECS.h"

#define MAX_STACK 5

struct Item {
    ItemID itemID;
    int count = 1;
    bool stack = false;
    Item(ItemID i) {
        itemID = i;
    }
    virtual void render(int x, int y, int scale, bool inv = true) {}
    virtual bool onClick(int b) { return false; }
};

struct ItemStack : public Item {
    ItemStack(ItemID i, int c = 1) : Item(i) {
        count = c;
        stack = true;
    }
    void render(int x, int y, int scale, bool inv = true) override {
        TextureManager::drawTexture(TextureManager::getTexture("itemSheet.png"), BIT*((int)itemID % 3), BIT*((int)itemID / 3), BIT, BIT, x, y, scale, scale, true);
        if (count != 1 && inv) {
            TextManager::drawText(std::to_string(count), {x+scale/4,y+15});
        }
    }
};

struct ItemContainer {
    
    Item* item = nullptr;
    
    bool similar(ItemContainer* other) {
        return (item != nullptr && other -> item != nullptr && item -> stack && other -> item -> stack && item -> itemID == other -> item -> itemID);
    }
    
    bool giveHalf(ItemContainer* other) {
        if (other -> item != nullptr) { return false; }
        if (item == nullptr || !item -> stack || item -> count == 1) {
            other -> item = item;
            item = nullptr;
            return true;
        }
        int c = (item -> count + 1) / 2;
        item -> count -= c;
        other -> item = new ItemStack(item -> itemID, c);
        return true;
    }
    
    bool takeFull(ItemContainer* other) {
        if (similar(other)) {
            item -> count += other -> item -> count;
            other -> item = nullptr;
            return true;
        } else if (item == nullptr) {
            item = other -> item;
            other -> item = nullptr;
            return true;
        }
        return false;
    }
    
    bool addOne(ItemContainer* other) {
        if (item == nullptr) {
            if (other -> item == nullptr) {
                return false;
            }
            if (!other -> item -> stack) {
                item = other -> item;
                other -> item = nullptr;
                return true;
            }
            item = new ItemStack(other -> item -> itemID, 1);
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
    ItemID itemID;
    Loot(ItemID i, int a = 1, int b = 1) {
        min = a;
        max = b;
        itemID = i;
    }
    Item* createItem() {
        return new ItemStack(itemID, min+(rand() % (max+1-min)));
    }
};

struct LootTable {
    v(Loot) table;
    LootTable() {}
    void addLoot(ItemID i, int a = 1, int b = 1) {
        assert(a <= b);
        table.emplace_back(i, a, b);
    }
};

class Consumable : public ItemStack {
public:
    Consumable(ItemID itemID, int c) : ItemStack(itemID, c) {};
    bool onClick(int b) override {
        if (b == SDL_BUTTON_RIGHT) {
            count -= 1;
            LOG("Item of type",(int)itemID,"consumed");
        }
        return true;
    }
};
