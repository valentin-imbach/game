//
//  GuiManager.hpp
//  Game
//
//  Created by Valentin Imbach on 10.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "GuiElements.hpp"

class GuiManager2 {
private:
    v(GuiElement*) guiElements;
public:
    static GuiManager2* manager;
    ItemContainer* mouseContainer = new ItemContainer();
    ItemSlot* mouseSlot = new ItemSlot({0,0}, mouseContainer);
    
    GuiManager2() { manager = this; }
    void addGuiElement(GuiElement* gui);
    void update();
    void render();
    bool handleEvent(SDL_Event event);
    
};
