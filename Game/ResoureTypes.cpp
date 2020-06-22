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
    types[(int)RESOURCE::STONE] = new ResourceType({0,4},{1,1},1,11);
    types[(int)RESOURCE::STONE] -> loot.addLoot(6,2,4);
    
    types[(int)RESOURCE::BUSH] = new ResourceType({3,1},{1,1},0);
    types[(int)RESOURCE::BUSH] -> loot.addLoot(17,1,2);
    
    types[(int)RESOURCE::TREE] = new ResourceType({0,3},{1,2},2,3);
    types[(int)RESOURCE::TREE] -> loot.addLoot(7,3,5);
}



