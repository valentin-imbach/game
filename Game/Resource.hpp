//
//  Resource.hpp
//  Game
//
//  Created by Valentin Imbach on 17.12.19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once
#include <stdio.h>
#include "GameObject.hpp"

class Resource : public GameObject {
public:
    Resource(int x, int y, int H, int sx, int sy);
};


