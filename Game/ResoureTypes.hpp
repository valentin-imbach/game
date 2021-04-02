//
//  ResoureTypes.hpp
//  Game
//
//  Created by Valentin Imbach on 16.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#define MAX_RESOURCE_TYPES 30

#include "Item.hpp"

enum class RESOURCE {
    STONE,
    BUSH,
    TREE,
    
    MAX
};

enum class PICKUP {
    BRANCH,
    PEBBLE,
    
    MAX
};

struct ResourceType {
    static ResourceType* types[MAX_RESOURCE_TYPES];
    static void setTypes();
    
    int vars;
    LootTable loot;
    pair<int> position;
    pair<int> size;
    int height;
    ItemID tool;
    const char* sound;
    
    ResourceType(pair<int> pos, pair<int> s, int h, ItemID t, int v = 1, const char* so = "") : tool(t), size(s), height(h), position(pos), vars(v), sound(so) {}
};

