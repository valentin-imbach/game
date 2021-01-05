//
//  InventoryComponent.h
//  Game
//
//  Created by Valentin Imbach on 28.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.hpp"
#include "Components.h"

class InventoryComponent : public Component {
public:
    static ComponentType componentType;
    pair<int> size;
    vv(ItemContainer) containers;
    
    InventoryComponent(int w = 1, int h = 1) : size(w,h) {
        containers = vv(ItemContainer)(size.X, v(ItemContainer)(size.Y));
    }
    
    Item* addItem(Item* item) {
        ItemContainer* itemContainer = new ItemContainer();
        itemContainer -> item = item;
        for (int y = 0; y < size.Y; y++) {
            for (int x = 0; x < size.X; x++)
                containers[x][y].takeFull(itemContainer);
        }
        return item;
    }
    
    SERIALIZE(size);
    
    Component* create() override {
        return new InventoryComponent();
    }
    
};
