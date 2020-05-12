//
//  GuiManager.cpp
//  Game
//
//  Created by Valentin Imbach on 10.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "GuiManager.hpp"

void GuiManager2::addGuiElement(GuiElement *gui) {
    guiElements.push_back(gui);
}

void GuiManager2::update() {
    for (GuiElement* gui : guiElements) { gui -> update(); }
}

void GuiManager2::render() {
    for (GuiElement* gui : guiElements) { gui -> render(); }
}

bool GuiManager2::handleEvent(SDL_Event event) {
    for (GuiElement* gui : guiElements) {
        if (gui -> handleEvent(event)) { return true; }
    }
    return false;
}
