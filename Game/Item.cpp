//
//  Item.cpp
//  Game
//
//  Created by Valentin Imbach on 10/12/2020.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "tools.h"
#include "Item.h"
#include "Item.hpp"

std::array<ItemTemplate*,(int)ItemID::MAX> Item::itemTemplates;
std::array<ItemTypeTemplate*,(int)ItemType::MAX> Item::itemTypeTemplates;
std::array<ItemPropertyTemplate*,(int)ItemProperty::MAX> Item::itemPropertyTemplates;

void Item::setTemplates() {
    
    //ITEMS
    itemTemplates[(int)ItemID::SHOVEL] = new ItemTemplate("Shovel",{ItemType::TOOL});
    itemTemplates[(int)ItemID::SHOVEL] -> properties[(int)ItemProperty::EFFICIENCY] = 7;
    
    itemTemplates[(int)ItemID::PICK] = new ItemTemplate("Pick Axe",{ItemType::TOOL});
    itemTemplates[(int)ItemID::HAMMER] = new ItemTemplate("Hammer",{ItemType::TOOL});
    
    itemTemplates[(int)ItemID::BRANCH] = new ItemTemplate("Branch",{ItemType::ROD, ItemType::SWORD, ItemType::FUEL});
    
    //TYPES
    itemTypeTemplates[(int)ItemType::TOOL] = new ItemTypeTemplate("Tool", {ItemProperty::EFFICIENCY});
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


