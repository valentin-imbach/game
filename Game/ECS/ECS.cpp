//
//  ECS.cpp
//  Game
//
//  Created by Valentin Imbach on 07.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "ECS.h"

void Entity::addTag(TAG tag) {
    tagBitSet[(int)tag] = true;
    manager -> taggedEntities[(int)tag].push_back(this);
}
