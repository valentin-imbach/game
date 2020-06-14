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
    TABLE,
    CHEST,
    TREE
};

class EntityFactory {
public:
    static void createEntity(EntityManager* manager, EntityType type, pair<int> pos);
};
