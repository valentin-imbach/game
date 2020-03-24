//
//  Inventory.cpp
//  Game
//
//  Created by Valentin Imbach on 17.12.19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Inventory.hpp"

Inventory::Inventory() {}

Inventory::Inventory(int w, int h) {
    dim = {w,h};
    slots = vv(ItemStack*)(w,v(ItemStack*)(h,nullptr));
}

void Inventory::render(int x, int y, int d, int scale) {
    for (int i = 0; i < dim.X; i++) {
        for (int j = 0; j < dim.Y; j++) {
            if (slots[i][j] == nullptr) {
                continue;
            }
            slots[i][j] -> render(x+i*d,y+j*d,scale);
        }
    }
}
