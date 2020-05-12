//
//  InventoryComponent.h
//  Game
//
//  Created by Valentin Imbach on 28.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.h"
#include "Components.h"

class InventoryComponent : public Component {
public:
    pair<int> size;
    vv(ItemContainer) itemSlots;
    
    InventoryComponent(Entity* entity, int w, int h) {
        size = {w,h};
        itemSlots = vv(ItemContainer)(w,v(ItemContainer)(h));
    }
    
    Item* addItem(Item* item) {
        ItemContainer* itemContainer = new ItemContainer();
        itemContainer -> item = item;
        for (int y = 0; y < size.Y; y++) {
            for (int x = 0; x < size.X; x++) {
                itemSlots[x][y].addFull(itemContainer);
            }
        }
        return item;
    }
};
