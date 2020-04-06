//
//  Game.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#include <stdio.h>

#include "Camera.hpp"
#include "Map.hpp"
#include "Components.h"

class Game {
public:
    Game();
    ~Game();
    void render();
    void update();
private:
    Map* world;
    EntityManager manager;
    Entity& player = manager.addEntity();
    Entity& wall = manager.addEntity();
};
