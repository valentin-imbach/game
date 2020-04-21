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
#include "GuiManager.hpp"
#include "LayerManager.h"
#include "EntityLayer.h"
#include "TileLayer.h"

class Game {
public:
    Game();
    ~Game();
    void render();
    void update();
    void handleEvents();
private:
    LayerManager layerManager;
    Entity* player;
};
