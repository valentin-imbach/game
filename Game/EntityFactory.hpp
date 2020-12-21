//
//  EntityFactory.hpp
//  Game
//
//  Created by Valentin Imbach on 11.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "ECS/Components.h"

enum class EntityType {
    TABLE = 100,
    CHEST,
    FURNACE,
    
    //Workstations
    ASSEMBLY_TABLE,
    CARPENTRY_TABLE,
    TOOL_FORGE,
    STONE_CUTTER,
    FLETCHING_STATION,
    GRINDER,
    ANVIL,
    PRESS,
    DESK,
    SINK,
    KILT,
    ALCHEMY_TABLE,
    CAULDRON,
    COOKING_POT,
    ALTAR,
    BOOKSHELF,
    CRYSTAL_BALL,
    KEG,
    LOOM,
    WELDER,
    MILL,
    FOUNTAIN,
    CAMPFIRE,
    COMOSTER
};

class EntityFactory {
public:
    static void createEntity(EntityManager* manager, int type, pair<int> pos);
};
