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
    vv(ItemSlot) itemSlots;
    InventoryComponent(int w, int h) {
        size = {w,h};
        itemSlots = vv(ItemSlot)(w,v(ItemSlot)(h));
    }
    Item* addItem(Item* item) {
        if (item == nullptr) {
            return nullptr;
        }
        for (int y = 0; y < size.Y; y++) {
            for (int x = 0; x < size.X; x++) {
                if (itemSlots[x][y].item == nullptr) {
                    itemSlots[x][y].item = item;
                    return nullptr;
                } else if (itemSlots[x][y].item -> stack && item -> stack) {
                    if (itemSlots[x][y].item -> type == item -> type) {
                        itemSlots[x][y].item -> count += item -> count;
                        delete item;
                        return nullptr;
                    }
                }
                
            }
        }
        return item;
    }
};
