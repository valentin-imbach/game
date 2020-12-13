//
//  GuiManager.cpp
//  Game
//
//  Created by Valentin Imbach on 10.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "GuiManager.hpp"

GuiManager* GuiManager::manager = nullptr;

void GuiManager::addGuiElement(GuiElement *gui) {
    guiElements.push_back(gui);
    gui -> setManager(this);
}

void GuiManager::update() {
    guiElements.erase(std::remove_if(std::begin(guiElements), std::end(guiElements),[](const GuiElement* gui) { return !(gui -> alive); }), std::end(guiElements));
    for (GuiElement* gui : guiElements) { gui -> update(); }
    mouseSlot -> position = Window::mousePos - pair<int>(24,24);
}

void GuiManager::render() {
    for (GuiElement* gui : guiElements) { gui -> render(); }
    mouseSlot -> render();
}

bool GuiManager::handleEvent(SDL_Event event) {
    if (event.key.repeat) { return false; }
    for (int i = (int)guiElements.size()-1; i >= 0; i--) {
        if (guiElements[i] -> handleEvent(event)) { return true; }
    }
    return false;
}
