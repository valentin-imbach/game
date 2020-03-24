//
//  Game.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "Window.hpp"

#include "Map.hpp"
#include "Camera.hpp"
#include "Player.hpp"

class Game {
public:
    Game();
    ~Game();
    void render();
    void update();
private:
    Map* world;
    Player* player;
};
