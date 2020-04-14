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
    pint size;
    vv(ItemStack*) items;
    InventoryComponent(int w, int h) {
        size = {w,h};
        items = vv(ItemStack*)(w,v(ItemStack*)(h,nullptr));
    }
    void render() override {
        int x = 200;
        int y = 200;
        int d = 60;
        int scale = 3;
        
        for (int i = 0; i < size.X; i++) {
            for (int j = 0; j < size.Y; j++) {
                if (items[i][j] != nullptr) {
                    items[i][j] -> render(x+i*d,y+j*d,scale);
                }
            }
        }
    }
};
