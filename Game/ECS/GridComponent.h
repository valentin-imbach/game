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
    static ComponentType componentType;
    pair<int> size;
    PositionComponent *positionComponent;

    GridComponent(pair<int> s = {1,1}) {
        size = s;
    }
    
    void init() override {
        positionComponent = entity -> getComponent<PositionComponent>();
        for (int i = 0; i < size.X; i++) {
            for (int j = 0; j < size.Y; j++) {
                entity -> manager -> gridEntities[positionComponent -> position.X+i][positionComponent -> position.Y-j] = entity;
            }
        }
        entity -> addTag(TAG::STRUCT);
    }
    
    Component* create() override {
        return new GridComponent();
    }
    
};
