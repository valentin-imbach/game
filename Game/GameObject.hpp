//
//  GameObject.hpp
//  Game
//
//  Created by Valentin Imbach on 17.12.19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "tools.h"
#include "Camera.hpp"

class GameObject {
public:
    GameObject();
    void update();
    void render();
    inline void setSprite(int a, int b) { sprite = {a,b}; }
protected:
    pint pos;
    pint dim;
    int height;
    pint sprite;
};
