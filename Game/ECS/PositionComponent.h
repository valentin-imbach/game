//
//  PositionComponent.h
//  Game
//
//  Created by Valentin Imbach on 26.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"

class PositionComponent : public Component {
public:
    static ComponentType componentType;
    pair<float> position;
    
    PositionComponent(pair<float> pos = {0,0}) {
        position = pos;
    }
    
    SERIALIZE(position);
    
    Component* create() override {
        return new PositionComponent();
    }
    
};

