//
//  Game.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once
#include "LayerSystem/Layers.h"
#include "Gui/GuiManager.hpp"
#include "Gui/PauseMenu.hpp"
#include "Console.hpp"
#include "StateController.h"
#include "World.hpp"

class Game {
public:
    Game();
    void render();
    void update();
    void handleEvents();
    
    StateController controller;

    World* world;
    PauseMenu* pauseMenu;
    Console* console;
};
