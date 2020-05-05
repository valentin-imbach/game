//
//  TableComponent.h
//  Game
//
//  Created by Valentin Imbach on 28.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.h"

class TableComponent : public Component {
public:
    ItemSlot a;
    ItemSlot b;
    ItemSlot c;
    
    TableComponent(Entity* entity, pair<int> pos) {
        entity -> addComponent<GridComponent>(pos);
    }
    
    bool handleEvent(SDL_Event) override {
        return false;
    }
};
