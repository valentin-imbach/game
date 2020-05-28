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
#include "Gui/MainMenu.hpp"
#include "Console.hpp"
#include "StateController.h"
#include "World.hpp"

class Game {
public:
    Game();
    void render();
    void update();
    void handleEvents();
    static void loadWorld();
    
    static StateController controller;

    static World* world;
    PauseMenu pauseMenu;
    MainMenu mainMenu;
    Console console;
};
