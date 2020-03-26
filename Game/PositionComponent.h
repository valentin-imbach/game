//
//  PositionComponent.h
//  Game
//
//  Created by Valentin Imbach on 26.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "ECS.h"

class PositionComponent : public Component {
    
public:
    int x;
    int y;
    
    void init() override {
        x = 0;
        y = 0;
    }
    
    void update() override {
        x++;
        y++;
    }
    
};
