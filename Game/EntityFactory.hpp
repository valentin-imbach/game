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
    ROCK,
    BUSH,
    TABLE = 100,
    CHEST,
    TREE
};

class EntityFactory {
public:
    static void createEntity(EntityManager* manager, int type, pair<int> pos);
};
