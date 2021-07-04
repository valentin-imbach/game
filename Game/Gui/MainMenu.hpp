//
//  MainMenu.hpp
//  Game
//
//  Created by Valentin Imbach on 28.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "GuiManager.hpp"

class MainMenu {
private:
    GuiElement* gui;
public:
    MainMenu();
    void render();
    bool handleEvent(SDL_Event event);
    
    void buttonPressed();
};
