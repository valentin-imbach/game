//
//  Resource.cpp
//  Game
//
//  Created by Valentin Imbach on 17.12.19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Resource.hpp"

Resource::Resource(int x, int y, int H, int sx, int sy) {
    pos = {x,y};
    dim = {1,1};
    height = H;
    sprite = {sx,sy};
}
