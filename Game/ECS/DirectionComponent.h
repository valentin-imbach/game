//
//  DirectionComponent.h
//  Game
//
//  Created by Valentin Imbach on 27.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"

class DirectionComponent : public Component {
public:
    static ComponentType componentType;
    Direction direction;

    DirectionComponent(Direction dir = SOUTH) : direction(dir) {}
    
    SERIALIZE(direction);
    
    Component* create() override {
        return new DirectionComponent();
    }
    
};
