//
//  ResourceComponent.h
//  Game
//
//  Created by Valentin Imbach on 28.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.h"

class ResourceComponent : public Component {
public:
    int type;
    ResourceComponent(int t) {
        type = t;
    }
    bool mine(Item* tool) {
        if (tool -> type == type) {
            entity -> active = false;
            return true;
        }
        return false;
    }
};
