//
//  GuiElements.cpp
//  Game
//
//  Created by Valentin Imbach on 10.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "GuiElements.hpp"
#include "../TextureManager.hpp"
#include "../Window.hpp"
#include "GuiManager.hpp"

GuiElement::GuiElement(pair<int> pos, pair<int> s, SDL_Texture* tex) {
    position = pos-s/2;
    size = s;
    texture = tex;
}

void GuiElement::setManager(GuiManager2* m) {
    manager = m;
    for (GuiElement* child : children) {
        child -> setManager(m);
    }
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
}

bool GuiElement::check(pair<int> p) {
    return (position.X <= p.X && p.X <= position.X+size.X && position.Y <= p.Y && p.Y <= position.Y+size.Y);
}

bool GuiElement::handleEvent(SDL_Event event) {
    
    for (int i = (int)children.size()-1; i >= 0; i--) {
        if (children[i] -> handleEvent(event)) {
            return true;
        }
    }
    
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if(onClick(event.button.button)) { return true; }
    } else if (event.type == SDL_KEYDOWN) {
        if (onKey(event.key.keysym.scancode)) { return true; }
    }
    return false;
}

void GuiElement::addGuiElement(GuiElement* gui) {
    children.push_back(gui);
    gui -> setParent(this);
    gui -> manager = manager;
}

void GuiElement::setParent(GuiElement *gui) {
    parent = gui;
    position += gui -> position;
}

void GuiElement::destroy() {
    alive = false;
    for (GuiElement* child : children) {
        child -> destroy();
    }
}

Widget::Widget(pair<int> pos, pair<int> s, SDL_Texture* tex) : GuiElement(pos,s,tex) {}

bool Widget::onKey(int key) {
    for (int i = (int)children.size()-1; i >= 0; i--) {
        if (children[i] -> onKey(key)) {
            return true;
        }
    }
    
    if (key == SDL_SCANCODE_E) {
        destroy();
        return true;
    }
    return false;
}

Button::Button(pair<int> pos, pair<int> s, SDL_Texture* tex) : GuiElement(pos,s,tex) {}

ItemSlot::ItemSlot(pair<int> pos, ItemContainer* item) : GuiElement(pos,{48,48}) {
    itemContainer = item;
}

bool ItemSlot::onClick(int button) {
    if (check(Window::mousePos)) {
        if (button == SDL_BUTTON_LEFT) {
            if (!itemContainer -> addFull(manager -> mouseContainer)) {
                std::swap(manager -> mouseContainer -> item, itemContainer -> item);
            }
        } else if (button == SDL_BUTTON_RIGHT) {
            if (!itemContainer -> takeHalf(manager -> mouseContainer)) {
                itemContainer -> addOne(manager -> mouseContainer);
            }
        }
        return true;
    }
    return false;
}

void ItemSlot::render() {
    itemContainer -> render(position+size/2,size.X);
}

