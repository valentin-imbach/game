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


