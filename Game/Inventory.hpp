//
//  Inventory.hpp
//  Game
//
//  Created by Valentin Imbach on 17.12.19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once
#include <stdio.h>
#include "tools.h"
#include "ItemStack.hpp"

struct Inventory {
    Inventory();
    Inventory(int w, int h);
    pint dim;
    vv(ItemStack*) slots;
    void render(int x, int y, int d, int scale);
};
