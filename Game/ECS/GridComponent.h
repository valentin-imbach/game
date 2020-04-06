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
    int x, y;
    Map* map;
    PositionComponent *positionComponent;
    CollisionComponent *collisionComponent;
    
    GridComponent(int x, int y, Map* m) {
        this -> x = x;
        this -> y = y;
        map = m;
        (map -> entities)[x][y] = entity;
    }
    
    void init() override {
        positionComponent = &entity -> addComponent<PositionComponent>(x,y);
        collisionComponent = &entity -> addComponent<CollisionComponent>();
    }
};
