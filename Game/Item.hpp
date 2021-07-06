//
//  Item.h
//  Game
//
//  Created by Valentin Imbach on 13.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "Item.h"
#include "Window.hpp"

#define MAX_STACK 5

struct ItemPropertyTemplate {
    std::string name;
    ItemPropertyTemplate(std::string n) : name(n) {}
};

struct ItemTypeTemplate {
    std::string name;
    v(ItemProperty) properties;
    ItemTypeTemplate(std::string n, v(ItemProperty) p) : name(n), properties(p) {}
};

struct ItemTemplate {
    std::string name;
    v(ItemType) types;
    std::bitset<(int)ItemType::MAX> typeBitSet;
    std::array<int,(int)ItemProperty::MAX> properties;
    ItemTemplate(std::string n, v(ItemType) t) : name(n), types(t) {
        for (ItemType type : t) typeBitSet[(int)type] = true;
    }
};

struct Item {
    ItemID itemID;
    int count;
    bool stack;
    ItemTemplate* temp;
    
    Item(ItemID i = ItemID::NONE, int c = 1) : itemID(i), count(c) {
        stack = itemID != ItemID::NONE;
    }
    
    bool hasType(ItemType t) {
        if (temp == nullptr) return false;
        return temp -> typeBitSet[(int)t];
    }
    
    virtual void render(int x, int y, int scale, bool inv = true) = 0;
    virtual bool onClick(bool attack) { return false; }
    
    static std::array<ItemTemplate*,(int)ItemID::MAX> itemTemplates;
    static std::array<ItemTypeTemplate*,(int)ItemType::MAX> itemTypeTemplates;
    static std::array<ItemPropertyTemplate*,(int)ItemProperty::MAX> itemPropertyTemplates;
    static void setTemplates();
};

struct ItemStack : public Item {
    ItemStack(ItemID i, int c = 1) : Item(i, c) {
        temp = itemTemplates[(int)itemID];
    }
    void render(int x, int y, int scale, bool inv = true) override;
};

struct Tool : public Item {
    pair<int> pos;
    Tool(std::string name, ItemType t, pair<int> p);
    void render(int x, int y, int scale, bool inv = true) override;
};

struct ItemContainer {
    
    ItemType type;
    Item* item = nullptr;
    
    ItemContainer(ItemType t = ItemType::NONE) : type(t) {}
    
    bool similar(ItemContainer* other) {
        if (item == nullptr || other -> item == nullptr) return false;
        if (!item -> stack || !other -> item -> stack) return false;
        return (item -> itemID == other -> item -> itemID);
    }
    
    bool giveHalf(ItemContainer* other) {
        if (other -> item != nullptr) return false;
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
            if (other -> item == nullptr) return false;
            if (!other -> item -> stack) {
                item = other -> item;
                other -> item = nullptr;
                return true;
            }
            item = new ItemStack(other -> item -> itemID, 1);
            other -> item -> count -= 1;
            if (other -> item -> count == 0) other -> item = nullptr;
            return true;
        }
        
        if (similar(other)) {
            item -> count += 1;
            other -> item -> count -= 1;
            if (other -> item -> count == 0) other -> item = nullptr;
            return true;
        }
        return false;
    }
    
    void render(pair<int> pos, int scale) {
        if (item != nullptr) item -> render(pos.X, pos.Y, scale);
    }
    
    void renderToolTip(pair<int> pos);
    
};

class CraftingIndex {
private:
    std::map<ItemID,std::pair<ItemID,int>> index;
    
public:
    
    void add(ItemID a, std::pair<ItemID, int> b) {
        index[a] = b;
    }
    
    int craft(ItemContainer* in, ItemContainer* out, int max = -1) {
        if (!in -> item) return 0;
        ItemID inID = in -> item -> itemID;
        if (index.find(inID) != index.end()) {
            int num = in -> item -> count;
            if (max != -1) num = std::min(num, max);
            out -> item = new ItemStack(index[inID].first, num * index[inID].second);
            in -> item = nullptr;
            return num;
        }
        return 0;
    }
};

struct Loot {
    int min, max;
    ItemID itemID;
    Loot(ItemID i, int a = 1, int b = 1) : min(a), max(b), itemID(i) {}
    
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

struct Consumable : public ItemStack {
    Consumable(ItemID itemID, int c) : ItemStack(itemID, c) {};
    bool onClick(bool attack) override {
        if (attack == false) {
            count -= 1;
            LOG("Item of type",(int)itemID,"consumed");
        }
        return true;
    }
};
