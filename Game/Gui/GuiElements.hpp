//
//  GuiElements.hpp
//  Game
//
//  Created by Valentin Imbach on 10.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "../Item.h"

class GuiElement {
protected:
    pair<int> position;
    pair<int> size;
    SDL_Texture* texture;
    GuiElement* parent = nullptr;
    v(GuiElement*) children;
    
public:
    bool alive = true;
    GuiElement(pair<int> pos, pair<int> s, SDL_Texture* tex = nullptr);
    
    void addGuiElement(GuiElement* gui);
    void setParent(GuiElement* gui);
    void destroy() { alive = false; }
    
    virtual void render();
    virtual void update();
    virtual bool handleEvent(SDL_Event event);
};

class Button : public GuiElement {
public:
    Button(pair<int> pos, pair<int> s, SDL_Texture* tex = nullptr);
};

class ItemSlot : public GuiElement {
private:
    ItemContainer* itemContainer;
public:
    ItemSlot(pair<int> pos, pair<int> s, ItemContainer* item);
    void render() override;
};
