//
//  ResoureTypes.hpp
//  Game
//
//  Created by Valentin Imbach on 16.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#define MAX_RESOURCE_TYPES 30

#include "Item.h"

enum class RESOURCE {
    STONE,
    BUSH,
    TREE
};

struct ResourceType {
    static ResourceType* types[MAX_RESOURCE_TYPES];
    static void setTypes();
    
    LootTable loot;
    int variations;
    pair<int> position;
    pair<int> size;
    int tool;
    
    ResourceType(pair<int> pos, pair<int> s, int t, int v = 1) {
        tool = t;
        size = s;
        position = pos;
        variations = v;
    }
};

