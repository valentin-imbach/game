//
//  Item.cpp
//  Game
//
//  Created by Valentin Imbach on 10/12/2020.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "tools.h"
#include "Item.hpp"
#include "TextureManager.hpp"
#include "TextManager.hpp"

std::array<ItemTemplate*,(int)ItemID::MAX> Item::itemTemplates;
std::array<ItemTypeTemplate*,(int)ItemType::MAX> Item::itemTypeTemplates;
std::array<ItemPropertyTemplate*,(int)ItemProperty::MAX> Item::itemPropertyTemplates;

void Item::setTemplates() {
    
    //ITEMS
    itemTemplates[(int)ItemID::SHOVEL] = new ItemTemplate("Iron Shovel",{ItemType::SHOVEL});
    itemTemplates[(int)ItemID::SHOVEL] -> properties[(int)ItemProperty::EFFICIENCY] = 7;
    
    itemTemplates[(int)ItemID::PICK] = new ItemTemplate("Iron Pick Axe",{ItemType::PICK_AXE});
    itemTemplates[(int)ItemID::HAMMER] = new ItemTemplate("Iron Hammer",{ItemType::HAMMER});
    
    itemTemplates[(int)ItemID::BRANCH] = new ItemTemplate("Branch",{ItemType::ROD, ItemType::SWORD, ItemType::FUEL});
    itemTemplates[(int)ItemID::BRANCH] -> properties[(int)ItemProperty::BURN_TIME] = 2;
    itemTemplates[(int)ItemID::BRANCH] -> properties[(int)ItemProperty::HEAT] = 3;
    itemTemplates[(int)ItemID::BRANCH] -> properties[(int)ItemProperty::ATTACK] = 4;
    itemTemplates[(int)ItemID::BRANCH] -> properties[(int)ItemProperty::PARRY] = 2;
    
    //TYPES
    itemTypeTemplates[(int)ItemType::SHOVEL] = new ItemTypeTemplate("Shovel", {ItemProperty::EFFICIENCY});
    itemTypeTemplates[(int)ItemType::HAMMER] = new ItemTypeTemplate("Hammer", {ItemProperty::EFFICIENCY});
    itemTypeTemplates[(int)ItemType::PICK_AXE] = new ItemTypeTemplate("Pick Axe", {ItemProperty::EFFICIENCY});
    itemTypeTemplates[(int)ItemType::SWORD] = new ItemTypeTemplate("Sword", {ItemProperty::ATTACK, ItemProperty::PARRY});
    itemTypeTemplates[(int)ItemType::ROD] = new ItemTypeTemplate("Rod", {ItemProperty::FLEXIBILITY, ItemProperty::STENGTH});
    itemTypeTemplates[(int)ItemType::FUEL] = new ItemTypeTemplate("Fuel", {ItemProperty::BURN_TIME, ItemProperty::HEAT});
    
    //PROPERTIES
    itemPropertyTemplates[(int)ItemProperty::EFFICIENCY] = new ItemPropertyTemplate("Efficiency");
    itemPropertyTemplates[(int)ItemProperty::ATTACK] = new ItemPropertyTemplate("Attack");
    itemPropertyTemplates[(int)ItemProperty::PARRY] = new ItemPropertyTemplate("Parry");
    itemPropertyTemplates[(int)ItemProperty::FLEXIBILITY] = new ItemPropertyTemplate("Flexibility");
    itemPropertyTemplates[(int)ItemProperty::STENGTH] = new ItemPropertyTemplate("Strength");
    itemPropertyTemplates[(int)ItemProperty::BURN_TIME] = new ItemPropertyTemplate("Burn Time");
    itemPropertyTemplates[(int)ItemProperty::HEAT] = new ItemPropertyTemplate("Heat");
};

void ItemStack::render(int x, int y, int scale, bool inv) {
    int a = BIT*((int)itemID % 3);
    int b = BIT*((int)itemID / 3);
    int offset = 0;
    if (!inv) offset = 5*sin((float)Window::ticks / 20);
    TextureManager::drawTexture(TextureManager::getTexture("itemSheet.png"), a, b, BIT, BIT, x, y+offset, scale, scale, true);
    if (count != 1 && inv) {
        std::string s = std::to_string(count);
        TextManager::drawText(s, {x+scale/4,y});
    }
}


void ItemContainer::renderToolTip(pair<int> pos) {
    if (item != nullptr && item -> itemID != ItemID::NONE) {
        ItemTemplate* it = Item::itemTemplates[(int)(item -> itemID)];
        if (it != nullptr) {
            v(Text) text;
            v(int) h;
            if (Window::keys[SDL_SCANCODE_LSHIFT]) {
                text.emplace_back(it -> name, TTF_STYLE_UNDERLINE);
                for (ItemType t : it -> types) {
                    ItemTypeTemplate* itt = Item::itemTypeTemplates[(int)t];
                    if (itt != nullptr) {
                        text.emplace_back("  " + itt -> name, TTF_STYLE_BOLD);
                        int s = (int)text.size();
                        h.push_back(s);
                        for (ItemProperty p : itt -> properties) {
                            ItemPropertyTemplate* ipt = Item::itemPropertyTemplates[(int)p];
                            if (ipt != nullptr) text.emplace_back(ipt -> name + ": " + std::to_string(it -> properties[(int)p]));
                        }
                    }
                }
            } else text.emplace_back(it -> name);
            pair<int> box = TextManager::textSize(text, 25);
            TextureManager::drawTexture(TextureManager::getTexture("grey.png"), pos.X, pos.Y, box.X+20, box.Y+5);
            for (int i = 0; i < h.size(); i++) {
                int x = (int)(it -> types[i]) % 8;
                int y = (int)(it -> types[i]) / 8;
                TextureManager::drawTexture(TextureManager::icons2, 16*x, 16*y, 16, 16, pos.X+5, pos.Y + (h[i]-1)*25, 32, 32);
            }
            TextManager::drawText(text, {pos.X+10, pos.Y}, 25);
        }
    }
}
