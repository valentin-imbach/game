//
//  ResoureTypes.cpp
//  Game
//
//  Created by Valentin Imbach on 16.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "ResoureTypes.hpp"

ResourceType* ResourceType::types[MAX_RESOURCE_TYPES];

void ResourceType::setTypes() {
    types[(int)RESOURCE::STONE] = new ResourceType({0,4}, {1,1}, 0, ItemType::PICK_AXE, 11, "assets/sound/stone.mp3");
    types[(int)RESOURCE::STONE] -> loot.addLoot(ItemID::GRANITE_COBBLE, 2, 4);
    
    types[(int)RESOURCE::BUSH] = new ResourceType({3,1}, {1,1}, 0, ItemType::SHOVEL, 1, "assets/sound/bush.mp3");
    types[(int)RESOURCE::BUSH] -> loot.addLoot(ItemID::APPLE, 1, 2);
    
    types[(int)RESOURCE::TREE] = new ResourceType({0,3}, {1,2}, 1, ItemType::AXE, 3, "assets/sound/tree.mp3");
    types[(int)RESOURCE::TREE] -> loot.addLoot(ItemID::ROSEWOOD_LOG, 3, 5);
}
