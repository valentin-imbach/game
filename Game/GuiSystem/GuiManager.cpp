//
//  GuiManager.cpp
//  Game
//
//  Created by Valentin Imbach on 23.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "GuiManager.hpp"
#include "../Window.hpp"

v(Gui*) GuiManager::guiStack = v(Gui*)();
ItemSlot* GuiManager::mouseSlot = new ItemSlot();
ItemSlot* GuiManager::hotbarSlot = nullptr;

void GuiManager::Init() {
    LOG("Gui Manager initialized");
}

void GuiManager::update() {
    guiStack.erase(std::remove_if(guiStack.begin(), guiStack.end(), [](Gui* g) { return !(g -> alive); }), guiStack.end());
    for (auto g : guiStack) {
        g -> update();
    }
}

void GuiManager::render() {
    for (auto g : guiStack) {
        g -> render();
    }
    if (mouseSlot -> item != nullptr) {
        mouseSlot -> item -> render(Window::mousePos.X, Window::mousePos.Y, 48);
    }
}

void GuiManager::addGui(Gui* gui) {
    guiStack.push_back(gui);
}

bool GuiManager::handleEvent(SDL_Event event) {
    for (int i = (int) guiStack.size()-1; i >= 0; i--) {
        if (guiStack[i] -> handleEvent(event)) {
            return true;
        }
    }
    return false;
}
