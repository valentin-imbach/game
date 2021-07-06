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
    
    //logs
    
    itemTemplates[(int)ItemID::PINE_LOG] = new ItemTemplate("Pine Log", {ItemType::LOG, ItemType::FUEL});
    itemTemplates[(int)ItemID::PINE_LOG] -> properties[(int)ItemProperty::HARDNESS] = 3;
    itemTemplates[(int)ItemID::PINE_LOG] -> properties[(int)ItemProperty::BURN_TIME] = 5;
    itemTemplates[(int)ItemID::PINE_LOG] -> properties[(int)ItemProperty::HEAT] = 4;
    
    itemTemplates[(int)ItemID::OAK_LOG] = new ItemTemplate("Oak Log", {ItemType::LOG, ItemType::FUEL});
    itemTemplates[(int)ItemID::OAK_LOG] -> properties[(int)ItemProperty::HARDNESS] = 6;
    itemTemplates[(int)ItemID::OAK_LOG] -> properties[(int)ItemProperty::BURN_TIME] = 8;
    itemTemplates[(int)ItemID::OAK_LOG] -> properties[(int)ItemProperty::HEAT] = 5;
    
    itemTemplates[(int)ItemID::ROSEWOOD_LOG] = new ItemTemplate("Rosewood Log", {ItemType::LOG, ItemType::FUEL});
    itemTemplates[(int)ItemID::ROSEWOOD_LOG] -> properties[(int)ItemProperty::HARDNESS] = 9;
    itemTemplates[(int)ItemID::ROSEWOOD_LOG] -> properties[(int)ItemProperty::BURN_TIME] = 8;
    itemTemplates[(int)ItemID::ROSEWOOD_LOG] -> properties[(int)ItemProperty::HEAT] = 6;
    
    itemTemplates[(int)ItemID::ASHWOOD_LOG] = new ItemTemplate("Ashwood Log", {ItemType::LOG, ItemType::FUEL});
    itemTemplates[(int)ItemID::ASHWOOD_LOG] -> properties[(int)ItemProperty::HARDNESS] = 7;
    itemTemplates[(int)ItemID::ASHWOOD_LOG] -> properties[(int)ItemProperty::BURN_TIME] = 5;
    itemTemplates[(int)ItemID::ASHWOOD_LOG] -> properties[(int)ItemProperty::HEAT] = 9;
    
    itemTemplates[(int)ItemID::SPRUCE_LOG] = new ItemTemplate("Spruce Log", {ItemType::LOG, ItemType::FUEL});
    itemTemplates[(int)ItemID::SPRUCE_LOG] -> properties[(int)ItemProperty::HARDNESS] = 7;
    itemTemplates[(int)ItemID::SPRUCE_LOG] -> properties[(int)ItemProperty::BURN_TIME] = 6;
    itemTemplates[(int)ItemID::SPRUCE_LOG] -> properties[(int)ItemProperty::HEAT] = 5;
    
    //planks
    
    itemTemplates[(int)ItemID::PINE_PLANK] = new ItemTemplate("Pine Plank", {ItemType::PLATE});
    
    itemTemplates[(int)ItemID::OAK_PLANK] = new ItemTemplate("Oak Plank", {ItemType::PLATE});
    
    itemTemplates[(int)ItemID::ROSEWOOD_PLANK] = new ItemTemplate("Rosewood Plank", {ItemType::PLATE});
    
    itemTemplates[(int)ItemID::ASHWOOD_PLANK] = new ItemTemplate("Ashwood Plank", {ItemType::PLATE});
    
    itemTemplates[(int)ItemID::SPRUCE_PLANK] = new ItemTemplate("Spruce Plank", {ItemType::PLATE});

    //sticks
    
    itemTemplates[(int)ItemID::PINE_STICK] = new ItemTemplate("Pine Stick", {ItemType::ROD});
    itemTemplates[(int)ItemID::PINE_STICK] -> properties[(int)ItemProperty::FLEXIBILITY] = 8;
    itemTemplates[(int)ItemID::PINE_STICK] -> properties[(int)ItemProperty::STENGTH] = 4;
    
    itemTemplates[(int)ItemID::OAK_STICK] = new ItemTemplate("Oak Stick", {ItemType::ROD});
    itemTemplates[(int)ItemID::OAK_STICK] -> properties[(int)ItemProperty::FLEXIBILITY] = 5;
    itemTemplates[(int)ItemID::OAK_STICK] -> properties[(int)ItemProperty::STENGTH] = 6;
    
    itemTemplates[(int)ItemID::ROSEWOOD_STICK] = new ItemTemplate("Rosewood Stick", {ItemType::ROD});
    itemTemplates[(int)ItemID::ROSEWOOD_STICK] -> properties[(int)ItemProperty::FLEXIBILITY] = 2;
    itemTemplates[(int)ItemID::ROSEWOOD_STICK] -> properties[(int)ItemProperty::STENGTH] = 9;
    
    itemTemplates[(int)ItemID::ASHWOOD_STICK] = new ItemTemplate("Ashwood Stick", {ItemType::ROD});
    itemTemplates[(int)ItemID::ASHWOOD_STICK] -> properties[(int)ItemProperty::FLEXIBILITY] = 4;
    itemTemplates[(int)ItemID::ASHWOOD_STICK] -> properties[(int)ItemProperty::STENGTH] = 7;
    
    itemTemplates[(int)ItemID::SPRUCE_STICK] = new ItemTemplate("Spruce Stick", {ItemType::ROD});
    itemTemplates[(int)ItemID::SPRUCE_STICK] -> properties[(int)ItemProperty::FLEXIBILITY] = 5;
    itemTemplates[(int)ItemID::SPRUCE_STICK] -> properties[(int)ItemProperty::STENGTH] = 7;
    
    //cobbles
    
    itemTemplates[(int)ItemID::GRANITE_COBBLE] = new ItemTemplate("Granite Cobble", {ItemType::COBBLE});
    itemTemplates[(int)ItemID::GRANITE_COBBLE] -> properties[(int)ItemProperty::HARDNESS] = 7;
    
    itemTemplates[(int)ItemID::BASALT_COBBLE] = new ItemTemplate("Basalt Cobble", {ItemType::COBBLE});
    itemTemplates[(int)ItemID::BASALT_COBBLE] -> properties[(int)ItemProperty::HARDNESS] = 9;
    
    itemTemplates[(int)ItemID::LIMESTONE_COBBLE] = new ItemTemplate("Limestone Cobble", {ItemType::COBBLE});
    itemTemplates[(int)ItemID::LIMESTONE_COBBLE] -> properties[(int)ItemProperty::HARDNESS] = 6;
    
    itemTemplates[(int)ItemID::SANDSTONE_COBBLE] = new ItemTemplate("Sandstone Cobble", {ItemType::COBBLE});
    itemTemplates[(int)ItemID::SANDSTONE_COBBLE] -> properties[(int)ItemProperty::HARDNESS] = 4;
    
    itemTemplates[(int)ItemID::CHALK_COBBLE] = new ItemTemplate("Chalk Cobble", {ItemType::COBBLE});
    itemTemplates[(int)ItemID::CHALK_COBBLE] -> properties[(int)ItemProperty::HARDNESS] = 2;
    
    //pebbles
    
    itemTemplates[(int)ItemID::GRANITE_PEBBLE] = new ItemTemplate("Granite Cobble", {ItemType::PEBBLE});
    itemTemplates[(int)ItemID::GRANITE_COBBLE] -> properties[(int)ItemProperty::HARDNESS] = 7;
    
    itemTemplates[(int)ItemID::BASALT_COBBLE] = new ItemTemplate("Basalt Cobble", {ItemType::PEBBLE});
    itemTemplates[(int)ItemID::BASALT_COBBLE] -> properties[(int)ItemProperty::HARDNESS] = 9;
    
    itemTemplates[(int)ItemID::LIMESTONE_COBBLE] = new ItemTemplate("Limestone Cobble", {ItemType::PEBBLE});
    itemTemplates[(int)ItemID::LIMESTONE_COBBLE] -> properties[(int)ItemProperty::HARDNESS] = 6;
    
    itemTemplates[(int)ItemID::SANDSTONE_COBBLE] = new ItemTemplate("Sandstone Cobble", {ItemType::PEBBLE});
    itemTemplates[(int)ItemID::SANDSTONE_COBBLE] -> properties[(int)ItemProperty::HARDNESS] = 4;
    
    itemTemplates[(int)ItemID::CHALK_COBBLE] = new ItemTemplate("Chalk Cobble", {ItemType::PEBBLE});
    itemTemplates[(int)ItemID::CHALK_COBBLE] -> properties[(int)ItemProperty::HARDNESS] = 2;
    
    //TYPES
    
    itemTypeTemplates[(int)ItemType::SHOVEL] = new ItemTypeTemplate("Shovel", {ItemProperty::EFFICIENCY});
    itemTypeTemplates[(int)ItemType::HAMMER] = new ItemTypeTemplate("Hammer", {ItemProperty::EFFICIENCY});
    itemTypeTemplates[(int)ItemType::PICK_AXE] = new ItemTypeTemplate("Pick Axe", {ItemProperty::EFFICIENCY});
    itemTypeTemplates[(int)ItemType::SWORD] = new ItemTypeTemplate("Sword", {ItemProperty::DAMAGE, ItemProperty::PARRY});
    itemTypeTemplates[(int)ItemType::ROD] = new ItemTypeTemplate("Rod", {ItemProperty::FLEXIBILITY, ItemProperty::STENGTH});
    itemTypeTemplates[(int)ItemType::FUEL] = new ItemTypeTemplate("Fuel", {ItemProperty::BURN_TIME, ItemProperty::HEAT});
    itemTypeTemplates[(int)ItemType::LOG] = new ItemTypeTemplate("Log", {ItemProperty::HARDNESS});
    itemTypeTemplates[(int)ItemType::PEBBLE] = new ItemTypeTemplate("Pebble", {ItemProperty::HARDNESS});
    itemTypeTemplates[(int)ItemType::COBBLE] = new ItemTypeTemplate("Cobble", {ItemProperty::HARDNESS});

    
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
    int a = (int)itemID % 6;
    int b = (int)itemID / 6;
    TextureManager::drawTexture(TextureManager::getTexture("itemSheet2.png"), a*BIT, b*BIT, BIT, BIT, x, y, scale, scale, true);
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
    TextureManager::drawTexture(TextureManager::getTexture("itemSheet2.png"), pos.X*BIT, pos.Y*BIT, BIT, BIT, x, y, scale, scale, true);
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
