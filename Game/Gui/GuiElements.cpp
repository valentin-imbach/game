//
//  GuiElements.cpp
//  Game
//
//  Created by Valentin Imbach on 10.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "GuiElements.hpp"
#include "../TextureManager.hpp"
#include "GuiManager.hpp"

GuiElement::GuiElement(pair<int> pos, pair<int> s, SDL_Texture* tex, SDL_Texture* tex2) {
    position = pos-s/2;
    size = s;
    texture = tex;
    hoverTexture = tex2;
}

void GuiElement::setManager(GuiManager* m) {
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
    if (hoverTexture != nullptr && check(Window::mousePos)) { TextureManager::drawTexture(hoverTexture, position.X, position.Y, size.X, size.Y); }
    else if (texture != nullptr) { TextureManager::drawTexture(texture, position.X, position.Y, size.X, size.Y); }
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
    } else if (event.type == SDL_MOUSEWHEEL) {
        if (onScroll(event.wheel.y)) { return true; }
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
    onDestroy();
    alive = false;
    for (GuiElement* child : children) {
        child -> destroy();
    }
}

Widget::Widget(pair<int> pos, pair<int> s, SDL_Texture* tex, GuiElement* l) : GuiElement(pos,s,tex) {
    link = l;
}

void Widget::onDestroy() {
    if (link != nullptr) {
        link -> destroy();
    }
}

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

TextElement::TextElement(pair<int> pos, std::string t, bool c) : GuiElement(pos) {
    text = t;
    centre = c;
}

void TextElement::render() {
    TextManager::drawText(text, position, centre);
}

DisplayElement::DisplayElement(pair<int> pos, int* v) : GuiElement(pos) {
    value = v;
}

void DisplayElement::render() {
    TextManager::drawText(std::to_string(*value), position, true);
}

Button::Button(pair<int> pos, pair<int> s, void(*func)(), SDL_Texture* tex, SDL_Texture* tex2) : GuiElement(pos,s,tex,tex2) {
    function = func;
}

bool Button::onClick(int b) {
    if (check(Window::mousePos)) {
        if (b == SDL_BUTTON_LEFT) {
            (*function)();
        }
        return true;
    }
    return false;
}

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

Hotbar::Hotbar(v(ItemContainer*) items, int* sel) : GuiElement({Window::size.X/2,60},{534, 78},TextureManager::getTexture("hotbar.png")) {
    hotbarContainers = items;
    selected = sel;
}

void Hotbar::render() {
    TextureManager::drawTexture(TextureManager::getTexture("hotbar.png"), position.X, position.Y, 534, 78);
    for (int i = 0; i < hotbarContainers.size(); i++) {
        if (!hotbarContainers[i] -> empty()) {
            hotbarContainers[i] -> render(position + pair<int>(39+i*57,39),48);
        }
    }
    TextureManager::drawRect(position +  pair<int>(12+(*selected)*57,12), {54,54});
}

bool Hotbar::onScroll(int y) {
    if (y < 0) { (*selected) += 1; }
    if (y > 0) { (*selected) -= 1; }
    *selected = (*selected + (int)hotbarContainers.size()) % hotbarContainers.size();
    return true;
}

bool Hotbar::onKey(int k) {
    switch(k) {
        case SDL_SCANCODE_1:
            *selected = 0; return true;
        case SDL_SCANCODE_2:
            *selected = 1; return true;
        case SDL_SCANCODE_3:
            *selected = 2; return true;
        case SDL_SCANCODE_4:
            *selected = 3; return true;
        case SDL_SCANCODE_5:
            *selected = 4; return true;
        case SDL_SCANCODE_6:
            *selected = 5; return true;
        case SDL_SCANCODE_7:
            *selected = 6; return true;
        case SDL_SCANCODE_8:
            *selected = 7; return true;
        case SDL_SCANCODE_9:
            *selected = 8; return true;
    }
    return false;
}

bool Hotbar::onClick(int b) {
    Item* item = hotbarContainers[*selected] -> item;
    if (item != nullptr) {
        return hotbarContainers[*selected] -> item -> onClick(b);
    }
    return false;
};

HealthBar::HealthBar(int* h) : GuiElement({Window::size.X-330,50},{0,0}) {
    health = h;
    heart = TextureManager::getTexture("heart.png");
}

void HealthBar::render() {
    int offset = 30;
    for (int i = 0; i < *health; i++) {
        TextureManager::drawTexture(heart, position.X + i*offset, position.Y, 21, 18);
    }
}
