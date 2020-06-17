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
    BUSH
};

struct ResourceType {
    static ResourceType* types[MAX_RESOURCE_TYPES];
    static void setTypes();
    
    LootTable loot;
    Sprite sprite;
    int tool;
    pair<int> size;
    
    ResourceType(LootTable l, Sprite s, int t, pair<int> si = {1,1}) {
        loot = l;
        sprite = s;
        tool = t;
        size = si;
    }
};

