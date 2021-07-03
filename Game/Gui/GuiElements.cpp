//
//  GuiElements.cpp
//  Game
//
//  Created by Valentin Imbach on 10.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "GuiElements.hpp"
#include "../TextureManager.hpp"
#include "../TextManager.hpp"
#include "GuiManager.hpp"

//Gui Element

GuiElement::GuiElement(pair<int> pos, pair<int> s, SDL_Texture* tex, Alignment align) : align(align), size(s) {
    position = pos;
    relative_pos = pos;
    texture = tex;
}

void GuiElement::update() {
    int x = ((int)align) % 3;
    int y = ((int)align) / 3;
    if (parent) {
        position = parent -> position + relative_pos;
        position.X += (x-1)*(parent -> size.X)/2;
        position.Y += (y-1)*(parent -> size.Y)/2;
    } else {
        position.X = x*(Window::size.X)/2;
        position.Y = y*(Window::size.Y)/2;
        position += relative_pos;
    }
    for (GuiElement* child : children) child -> update();
}

void GuiElement::render() {
    if (hoverTexture && check(Window::mousePos)) TextureManager::drawTexture(hoverTexture, position.X, position.Y, 3, true);
    else if (texture) TextureManager::drawTexture(texture, position.X, position.Y, 3, true);
    extraRender();
    for (GuiElement* child : children) child -> render();
    TextureManager::drawRect(position-size/2, size, 255, 255, 255);
}

bool GuiElement::check(pair<int> p) {
    return (abs(position.X-p.X) <= size.X/2 && abs(position.Y-p.Y) <= size.Y/2);
}

bool GuiElement::handleEvent(SDL_Event event) {
    for (int i = (int)children.size()-1; i >= 0; i--) if (children[i] -> handleEvent(event)) return true;
    
    if (event.type == SDL_TEXTINPUT) return onText(event.text.text);
    else if (event.type == SDL_MOUSEBUTTONDOWN) return onClick(event.button.button);
    else if (event.type == SDL_KEYDOWN) return onKey(event.key.keysym.scancode);
    else if (event.type == SDL_MOUSEWHEEL) return onScroll(event.wheel.y);
    else if (event.type == SDL_USEREVENT && event.user.code == (int)EventCode::HOVER) {
        if (check(Window::mousePos)) {
            hover = true;
            return absorbHover;
        }
    } else if (event.type == SDL_USEREVENT && event.user.code == (int)EventCode::KEYSTATE) return absorbKeystate;
    else if (event.type == SDL_USEREVENT && event.user.code == (int)EventCode::RESET) hover = false;
    return false;
}

void GuiElement::addGuiElement(GuiElement* gui) {
    children.push_back(gui);
    gui -> parent = this;
    gui -> update();
}

//Widget

bool Widget::onKey(int key) {    
    if (key == SDL_SCANCODE_E) {
        alive = false;
        return !weak;
    }
    return false;
}

//TextElement

TextElement::TextElement(pair<int> pos, std::string t, bool c) : GuiElement(pos) {
    text = t;
    centre = c;
}

void TextElement::extraRender() {
    TextManager::drawText(text, position, centre);
}

//DisplayElement

DisplayElement::DisplayElement(pair<int> pos, int* v) : GuiElement(pos) {
    value = v;
}

void DisplayElement::extraRender() {
    std::string s = std::to_string(*value);
    TextManager::drawText(s, position, true);
}

//Button

Button::Button(pair<int> pos, pair<int> s, void(*func)(), SDL_Texture* tex, SDL_Texture* tex2) : GuiElement(pos, s, tex) {
    hoverTexture = tex2;
    function = func;
}

bool Button::onClick(int b) {
    if (check(Window::mousePos)) {
        if (b == SDL_BUTTON_LEFT) (*function)();
        return true;
    }
    return false;
}

//TextField

TextField::TextField(pair<int> pos, pair<int> s, SDL_Texture* tex) : GuiElement(pos, s, tex) {}

void TextField::extraRender() {
    TextManager::drawText(text, position, true);
}

bool TextField::onClick(int b) {
    if (check(Window::mousePos)) {
        if (b == SDL_BUTTON_LEFT) active = true;
        return true;
    }
    return false;
}

bool TextField::onKey(int k) {
    if (!active) { return false; }
    if (k == SDL_SCANCODE_ESCAPE || k == SDL_SCANCODE_RETURN) active = false;
    else if (k == SDL_SCANCODE_BACKSPACE) {
        if (text.length() > 0) text = text.substr(0,text.length()-1);
    }
    return true;
}

bool TextField::onText(std::string t) {
    if (!active) return false;
    text += t;
    return true;
}

//ItemSlot

ItemSlot::ItemSlot(pair<int> pos, ItemContainer* c) : GuiElement(pos, {48,48}), itemContainer(c) {}

bool ItemSlot::onClick(int button) {
    ItemContainer* mouseContainer = GuiManager::manager -> mouseSlot -> itemContainer;
    if (check(Window::mousePos)) {
        if (button == SDL_BUTTON_LEFT) {
            if (!itemContainer -> takeFull(mouseContainer)) std::swap(mouseContainer -> item, itemContainer -> item);
        } else if (button == SDL_BUTTON_RIGHT) {
            if (!itemContainer -> giveHalf(mouseContainer)) itemContainer -> addOne(mouseContainer);
        }
        return true;
    }
    return false;
}

void ItemSlot::extraRender() {
    TextureManager::drawTexture(TextureManager::getTexture("slot.png"), position.X, position.Y, 3, true);
    if (itemContainer -> item == nullptr) {
        ItemType type = itemContainer -> type;
        if (type != ItemType::NONE) {
            int x = (int)type % 8; int y = (int)type/8;
            TextureManager::drawTexture(TextureManager::icons, 16*x, 16*y, 16, 16, position.X, position.Y, 3, true);
        }
    }
    itemContainer -> render(position, 48);
    ItemContainer* mouseContainer = GuiManager::manager -> mouseSlot -> itemContainer;
    if (hover && mouseContainer -> item == nullptr) itemContainer -> renderToolTip({position.X+24,position.Y-24});
}

MouseSlot::MouseSlot() : ItemSlot({0,0}, new ItemContainer()) {}

void MouseSlot::update() {
    position = Window::mousePos - pair<int>(24,24);
}

void MouseSlot::extraRender() {
    itemContainer -> render(position+size/2,size.X);
}

//Hotbar

Hotbar::Hotbar(v(ItemContainer*) items, int* sel) : GuiElement({0,60}, {546, 78}, TextureManager::getTexture("hotbar.png"), Alignment::NORTH) {
    hotbarContainers = items;
    selected = sel;
}

void Hotbar::extraRender() {
    TextureManager::drawTexture(TextureManager::getTexture("HotbarHighlight.png"), position.X+(*selected-3)*78, position.Y, 3, true);
    for (int i = 0; i < hotbarContainers.size(); i++)
        if (hotbarContainers[i] -> item) hotbarContainers[i] -> render(position + pair<int>((i-3)*78),48);
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
    }
    return false;
}

bool Hotbar::onClick(int b) {
    Item* item = hotbarContainers[*selected] -> item;
    if (item != nullptr) return hotbarContainers[*selected] -> item -> onClick(b);
    return false;
};

//HealthBar

HealthBar::HealthBar(int* h) : GuiElement({-330,50}, {0,0}, nullptr, Alignment::NORTH_EAST), health(h) {
    heart = TextureManager::getTexture("heart.png");
}

void HealthBar::extraRender() {
    int offset = 30;
    for (int i = 0; i < *health; i++) TextureManager::drawTexture(heart, position.X + i*offset, position.Y, 21, 18);
}
