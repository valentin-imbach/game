//
//  GuiElements.cpp
//  Game
//
//  Created by Valentin Imbach on 10.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "GuiElements.hpp"
#include "../TextureManager.hpp"

void GuiElement::render() {
    if (parent == nullptr) {
        TextureManager::drawTexture(texture, position.X, position.Y, size.X, size.Y);
    } else {
        TextureManager::drawTexture(texture, position.X + parent -> position.X, position.Y + parent -> position.Y, size.X, size.Y);
    }
    for (GuiElement* child : children) {
        child -> render();
    }
}

bool GuiElement::handleEvent(SDL_Event event) {
    for (GuiElement* child : children) {
        if (child -> handleEvent(event)) {
            return true;
        }
    }
    return false;
}
