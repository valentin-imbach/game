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
    pair<int> size;
    PositionComponent *positionComponent;
    CollisionComponent *collisionComponent;
    
    GridComponent() {}
    GridComponent(Entity* entity, pair<int> pos = {0,0}, pair<int> s = {1,1}) {
        positionComponent = entity -> addComponent<PositionComponent>(pos);
        collisionComponent = entity -> addComponent<CollisionComponent>(s.X-0.5,s.Y-0.5,0.5,0.5);
        position = pos;
        size = s;
        for (int i = 0; i < s.X; i++) {
            for (int j = 0; j < s.Y; j++) {
                entity -> manager -> gridEntities[pos.X+i][pos.Y-j] = entity;
            }
        }
        entity -> addTag(TAG::STRUCT);
    }
};
