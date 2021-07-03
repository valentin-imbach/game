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
    itemTemplates[(int)ItemID::BRANCH] = new ItemTemplate("Oak Branch", {ItemType::ROD, ItemType::FUEL});
    itemTemplates[(int)ItemID::BRANCH] -> properties[(int)ItemProperty::FLEXIBILITY] = 1;
    itemTemplates[(int)ItemID::BRANCH] -> properties[(int)ItemProperty::STENGTH] = 7;
    itemTemplates[(int)ItemID::BRANCH] -> properties[(int)ItemProperty::BURN_TIME] = 2;
    itemTemplates[(int)ItemID::BRANCH] -> properties[(int)ItemProperty::HEAT] = 3;
    
    itemTemplates[(int)ItemID::WOOD] = new ItemTemplate("Oak Log", {ItemType::LOG, ItemType::FUEL});
    itemTemplates[(int)ItemID::WOOD] -> properties[(int)ItemProperty::BURN_TIME] = 2;
    itemTemplates[(int)ItemID::WOOD] -> properties[(int)ItemProperty::HEAT] = 3;
    itemTemplates[(int)ItemID::WOOD] -> properties[(int)ItemProperty::HARDNESS] = 6;
    
    itemTemplates[(int)ItemID::STONE] = new ItemTemplate("Granite", {ItemType::STONE});
    itemTemplates[(int)ItemID::STONE] -> properties[(int)ItemProperty::HARDNESS] = 6;
    
    //TYPES
    itemTypeTemplates[(int)ItemType::SHOVEL] = new ItemTypeTemplate("Shovel", {ItemProperty::EFFICIENCY});
    itemTypeTemplates[(int)ItemType::HAMMER] = new ItemTypeTemplate("Hammer", {ItemProperty::EFFICIENCY});
    itemTypeTemplates[(int)ItemType::PICK_AXE] = new ItemTypeTemplate("Pick Axe", {ItemProperty::EFFICIENCY});
    itemTypeTemplates[(int)ItemType::SWORD] = new ItemTypeTemplate("Sword", {ItemProperty::DAMAGE, ItemProperty::PARRY});
    itemTypeTemplates[(int)ItemType::ROD] = new ItemTypeTemplate("Rod", {ItemProperty::FLEXIBILITY, ItemProperty::STENGTH});
    itemTypeTemplates[(int)ItemType::FUEL] = new ItemTypeTemplate("Fuel", {ItemProperty::BURN_TIME, ItemProperty::HEAT});
    itemTypeTemplates[(int)ItemType::LOG] = new ItemTypeTemplate("Log", {ItemProperty::HARDNESS});
    itemTypeTemplates[(int)ItemType::STONE] = new ItemTypeTemplate("Stone", {ItemProperty::HARDNESS});

    
    //PROPERTIES
    itemPropertyTemplates[(int)ItemProperty::EFFICIENCY] = new ItemPropertyTemplate("Efficiency");
    itemPropertyTemplates[(int)ItemProperty::DAMAGE] = new ItemPropertyTemplate("Damage");
    itemPropertyTemplates[(int)ItemProperty::PARRY] = new ItemPropertyTemplate("Parry");
    itemPropertyTemplates[(int)ItemProperty::FLEXIBILITY] = new ItemPropertyTemplate("Flexibility");
    itemPropertyTemplates[(int)ItemProperty::STENGTH] = new ItemPropertyTemplate("Strength");
    itemPropertyTemplates[(int)ItemProperty::BURN_TIME] = new ItemPropertyTemplate("Burn Time");
    itemPropertyTemplates[(int)ItemProperty::HEAT] = new ItemPropertyTemplate("Heat");
    itemPropertyTemplates[(int)ItemProperty::HARDNESS] = new ItemPropertyTemplate("Hardness");
};

void ItemStack::render(int x, int y, int scale, bool inv) {
    int a = (int)itemID % 3;
    int b = (int)itemID / 3;
    TextureManager::drawTexture(TextureManager::getTexture("itemSheet.png"), a*BIT, b*BIT, BIT, BIT, x, y, scale, scale, true);
    if (count != 1 && inv) {
        std::string s = std::to_string(count);
        TextManager::drawText(s, {x+scale/4,y});
    }
}

Tool::Tool(std::string name, ItemType t, pair<int> p) {
    temp = new ItemTemplate(name, {t});
    pos = p;
}

void Tool::render(int x, int y, int scale, bool inv) {
    TextureManager::drawTexture(TextureManager::getTexture("itemSheet.png"), pos.X*BIT, pos.Y*BIT, BIT, BIT, x, y, scale, scale, true);
}

void ItemContainer::renderToolTip(pair<int> pos) {
    if (item != nullptr) {
        ItemTemplate* it = item -> temp;
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
