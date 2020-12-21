//
//  Item.h
//  Game
//
//  Created by Valentin Imbach on 13.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "Item.h"

#include "TextureManager.hpp"
#include "TextManager.hpp"
#include "Window.hpp"

#define MAX_STACK 5

struct ItemPropertyTemplate {
    std::string name;
    ItemPropertyTemplate(std::string n) : name(n) {}
};

struct ItemTypeTemplate {
    std::string name;
    v(ItemProperty) properties;
    ItemTypeTemplate(std::string n, v(ItemProperty) p = {}) : name(n), properties(p) {}
};

struct ItemTemplate {
    std::string name;
    v(ItemType) types;
    std::array<int,(int)ItemProperty::MAX> properties = {};
    ItemTemplate(std::string n, v(ItemType) t = {}) : name(n), types(t) {}
};

struct Item {
    ItemID itemID;
    int count = 1;
    bool stack = false;
    
    Item(ItemID i, int c = 1, bool s = false) : itemID(i), count(c), stack(s) {}
    
    virtual void render(int x, int y, int scale, bool inv = true) {}
    virtual bool onClick(int b) { return false; }
    
    static std::array<ItemTemplate*,(int)ItemID::MAX> itemTemplates;
    static std::array<ItemTypeTemplate*,(int)ItemType::MAX> itemTypeTemplates;
    static std::array<ItemPropertyTemplate*,(int)ItemProperty::MAX> itemPropertyTemplates;
    static void setTemplates();
};

struct ItemStack : public Item {
    ItemStack(ItemID i, int c = 1) : Item(i,c,true) {}
    
    void render(int x, int y, int scale, bool inv = true) override {
        int a = BIT*((int)itemID % 3);
        int b = BIT*((int)itemID / 3);
        TextureManager::drawTexture(TextureManager::getTexture("itemSheet.png"), a, b, BIT, BIT, x, y, scale, scale, true);
        if (count != 1 && inv) {
            std::string s = std::to_string(count);
            TextManager::drawText(s, {x+scale/4,y});
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
    
    void renderToolTip(pair<int> pos) {
        if (item != nullptr && item -> itemID != ItemID::NONE) {
            ItemTemplate* it = Item::itemTemplates[(int)(item -> itemID)];
            if (it != nullptr) {
                v(Text) text;
                if (Window::keys[SDL_SCANCODE_LSHIFT]) {
                    text.emplace_back(it -> name, TTF_STYLE_UNDERLINE);
                    for (ItemType t : it -> types) {
                        ItemTypeTemplate* itt = Item::itemTypeTemplates[(int)t];
                        if (itt != nullptr) {
                            text.emplace_back(itt -> name, TTF_STYLE_BOLD);
                            for (ItemProperty p : itt -> properties) {
                                ItemPropertyTemplate* ipt = Item::itemPropertyTemplates[(int)p];
                                if (ipt != nullptr) {
                                    text.emplace_back(ipt -> name + ": " + std::to_string(it -> properties[(int)p]));
                                }
                            }
                        }
                    }
                } else {
                    text.emplace_back(it -> name);
                }
                pair<int> box = TextManager::textSize(text, 25);
                TextureManager::drawTexture(TextureManager::getTexture("grey.png"), pos.X, pos.Y, box.X+20, box.Y+5);
                TextManager::drawText(text, {pos.X+10, pos.Y}, 25);
            }
        }
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
    bool onClick(int b) override {
        if (b == SDL_BUTTON_RIGHT) {
            count -= 1;
            LOG("Item of type",(int)itemID,"consumed");
        }
        return true;
    }
};
