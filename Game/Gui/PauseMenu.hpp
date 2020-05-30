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
    static void zoomLeft();
    static void zoomRight();
    static void fpsLeft();
    static void fpsRight();
    static void musicLeft();
    static void musicRight();
public:
    PauseMenu();
    void render();
    bool handleEvent(SDL_Event event);
};
