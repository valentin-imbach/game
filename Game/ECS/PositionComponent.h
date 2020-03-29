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
    float x;
    float y;
    PositionComponent() {
        x = 0;
        y = 0;
    }
    PositionComponent(float xpos, float ypos) {
        x = xpos;
        y = ypos;
    }
};
