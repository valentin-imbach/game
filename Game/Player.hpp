//
//  Player.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-25.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#include "tools.h"
#include "Camera.hpp"
#include "Map.hpp"
#include "Inventory.hpp"

class Player {
public:
    Player(float x, float y, int d);
    void render();
    void renderHotbar(int scale);
    void update(Map* world);
private:
    bool checkCollision(pout disp, Map* world);
    pout pos;
    float speed;
    int dir;
    int walkTicks;
    Inventory inv;
};
