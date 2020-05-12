//
//  GuiElements.cpp
//  Game
//
//  Created by Valentin Imbach on 10.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "GuiElements.hpp"
#include "../TextureManager.hpp"

GuiElement::GuiElement(pair<int> pos, pair<int> s, SDL_Texture* tex) {
    position = pos;
    size = s;
    texture = tex;
}

void GuiElement::update() {
    for (GuiElement* child : children) {
        child -> update();
    }
}

void GuiElement::render() {
    if (texture != nullptr) { TextureManager::drawTexture(texture, position.X, position.Y, size.X, size.Y); }
    for (GuiElement* child : children) {
        child -> render();
    }
    TextureManager::drawRect(position, size);
}

bool GuiElement::handleEvent(SDL_Event event) {
    for (GuiElement* child : children) {
        if (child -> handleEvent(event)) {
            return true;
        }
    }
    return false;
}

void GuiElement::addGuiElement(GuiElement* gui) {
    children.push_back(gui);
    gui -> setParent(this);
}

void GuiElement::setParent(GuiElement *gui) {
    parent = gui;
    position += gui -> position;
}




Button::Button(pair<int> pos, pair<int> s, SDL_Texture* tex) : GuiElement(pos,s,tex) {}

ItemSlot::ItemSlot(pair<int> pos, pair<int> s, ItemContainer* item) : GuiElement(pos,s) {
    itemContainer = item;
}

void ItemSlot::render() {}
