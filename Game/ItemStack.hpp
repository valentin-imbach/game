//
//  ItemStack.hpp
//  Game
//
//  Created by Valentin Imbach on 17.12.19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "tools.h"

#define MAX_STACK 5

struct ItemStack {
    ItemStack();
    ItemStack(int t, int c);
    int type;
    int count;
    int add(ItemStack stack);
    void render(int x, int y, float scale);
};
