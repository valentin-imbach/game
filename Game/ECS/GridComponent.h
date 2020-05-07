//
//  GridComponent.h
//  Game
//
//  Created by Valentin Imbach on 06.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "Components.h"

class GridComponent : public Component {
public:
    pair<int> position;
    PositionComponent *positionComponent;
    CollisionComponent *collisionComponent;
    
    GridComponent(Entity* entity, pair<int> pos) {
        positionComponent = entity -> addComponent<PositionComponent>(pos);
        collisionComponent = entity -> addComponent<CollisionComponent>();
        position = pos;
        entity -> manager -> gridEntities[pos.X][pos.Y] = entity;
        entity -> addTag(TAG::STRUCT);
    }
};
