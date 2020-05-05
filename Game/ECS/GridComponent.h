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
    
    GridComponent(pair<int> pos) {
        position = pos;
    }
    
    void init() override {
        positionComponent = &entity -> addComponent<PositionComponent>(position);
        collisionComponent = &entity -> addComponent<CollisionComponent>();
    }
};
