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
    vv(Item*) items;
    InventoryComponent(int w, int h) {
        size = {w,h};
        items = vv(Item*)(w,v(Item*)(h,nullptr));
    }
    Item* addItem(Item* item) {
        if (item == nullptr) {
            return nullptr;
        }
        for (int y = 0; y < size.Y; y++) {
            for (int x = 0; x < size.X; x++) {
                if (items[x][y] == nullptr) {
                    items[x][y] = item;
                    return nullptr;
                } else if (items[x][y] -> stack && item -> stack) {
                    if (items[x][y] -> type == item -> type) {
                        items[x][y] -> count += item -> count;
                        delete item;
                        return nullptr;
                    }
                }
                
            }
        }
        return item;
    }
};
