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
    vv(ItemStack*) items;
    InventoryComponent(int w, int h) {
        size = {w,h};
        items = vv(ItemStack*)(w,v(ItemStack*)(h,nullptr));
    }
};
