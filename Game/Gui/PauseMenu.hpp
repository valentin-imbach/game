//
//  PauseMenu.hpp
//  Game
//
//  Created by Valentin Imbach on 26.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "GuiManager.hpp"

class PauseMenu {
private:
    GuiElement* gui;
    void zoomLeft();
    void zoomRight();
    void fpsLeft();
    void fpsRight();
    void musicLeft();
    void musicRight();
public:
    PauseMenu();
    void render();
    bool handleEvent(SDL_Event event);
};
