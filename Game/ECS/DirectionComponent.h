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
    Direction direction;
    DirectionComponent(Direction dir = SOUTH) {
        direction = dir;
    }
};
